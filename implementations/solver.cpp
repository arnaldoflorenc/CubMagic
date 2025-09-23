
#include "../class/Solver.h"
#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <stack>
#include <memory>
#include <queue>
#include <unordered_set>
#include <list>
#include "../class/bloom.h"

using namespace std;

vector<M> get_moves(){
    return{
        {0, [](Cubo& c){c.rota_frente(); }, 1},
        {1, [](Cubo& c){c.rota_costa(); },1},
        {2, [](Cubo& c){c.rota_dir(); },1},
        {3, [](Cubo& c){c.rota_esq(); },1},
        {4, [](Cubo& c){c.rota_topo(); },1},
        {5, [](Cubo& c){c.rota_base(); },1}
    };
}

int inverso(int mov) {
    return mov ^ 1;
}

// Função para reconstruir o caminho como vetor de int
vector<int> reconstruir_caminho(shared_ptr<N> no_final) {
    vector<int> caminho;
    while (no_final && no_final->mov != -1) {
        caminho.push_back(no_final->mov);
        no_final = no_final->anterior;
    }
    reverse(caminho.begin(), caminho.end());
    return caminho;
}

vector<int> DFS(Cubo inicio, size_t bloomSize = 4000000, int bloomHashes = 5){ //busca uma solução enquanto n tiver chego no limite estabelecido
    auto movimentos = get_moves();
    unordered_set<size_t> visitados;
    stack<shared_ptr<N>> pilha;
    pilha.push(make_shared<N>(N{inicio, 0, -1, nullptr}));
    while(!pilha.empty()){
        auto no_atual = pilha.top();
        pilha.pop();
        if(no_atual->cube.resolvido()){
            return reconstruir_caminho(no_atual);
        }
        if(no_atual->profund >= 14){
            continue;
        }
        size_t estado_hash = no_atual->cube.hash();
        if(visitados.count(estado_hash)) {
            continue;
        }
        visitados.insert(estado_hash);
        for(auto &mov : movimentos){
            // Evita movimentos inversos
            if (no_atual->mov != -1 && mov.mov == inverso(no_atual->mov)) continue;
            // Evita ciclos curtos: exemplo, não faz A-B-A
            if (no_atual->anterior && no_atual->anterior->mov == mov.mov) continue;
            // Reutiliza Cubo por referência para evitar cópia desnecessária
            shared_ptr<Cubo> prox_cubo = make_shared<Cubo>(no_atual->cube);
            mov.acao(*prox_cubo);
            pilha.push(make_shared<N>(N{*prox_cubo, no_atual->profund + 1, mov.mov, no_atual}));
        }
    }
    return {};
}

vector<int> BFS(Cubo inicio, size_t bloomSize = 4000000, int bloomHashes = 5){
    auto movimentos = get_moves();
    bloom bloomFilter(bloomSize, bloomHashes);
    unordered_set<size_t> exactSet;
    size_t exactSetInsertions = 0;
    const size_t exactSetMaxSize = 100000; // Limpa a cada 100.000 inserções
    queue<shared_ptr<N>> gila;
    gila.push(make_shared<N>(N{inicio, 0, -1, nullptr}));
    while(!gila.empty()){
        auto no_atual = gila.front();
        gila.pop();
        if(no_atual->cube.resolvido()){
            return reconstruir_caminho(no_atual);
        }
        if(no_atual->profund >= 14){
            continue;
        }
        size_t estado_hash = no_atual->cube.hash();
        if(bloomFilter.mightContain(estado_hash)) {
            if(exactSet.count(estado_hash)) {
                continue;
            }
        } else {
            bloomFilter.insert(estado_hash);
            exactSet.insert(estado_hash);
            exactSetInsertions++;
            if (exactSetInsertions >= exactSetMaxSize) {
                exactSet.clear();
                exactSetInsertions = 0;
            }
        }
        for(auto &mov : movimentos){
            if (no_atual->mov != -1 && mov.mov == inverso(no_atual->mov)) continue;
            Cubo prox = no_atual->cube;
            mov.acao(prox);
            gila.push(make_shared<N>(N{prox, no_atual->profund + mov.custo, mov.mov, no_atual}));
        }
    }
    return {};
}

vector<int> Aestrela(Cubo inicio, size_t bloomSize, int bloomHashes) {
    auto movimentos = get_moves();
    bloom bloomFilter(bloomSize, bloomHashes);
    unordered_set<size_t> exactSet;
    size_t exactSetInsertions = 0;
    const size_t exactSetMaxSize = 100000;
    list<shared_ptr<A>> openList;
    openList.push_back(make_shared<A>(inicio, 0, -1, nullptr, 0, 0, 0));
    while (!openList.empty()) {
        auto no_atual = openList.front();
        openList.pop_front();

        if (no_atual->cube.resolvido()) {
            return reconstruir_caminho(static_pointer_cast<N>(no_atual));
        }
        if (no_atual->g >= 14) {
            continue;
        }

        size_t estado_hash = no_atual->cube.hash();
        // Controle de visitados: só expanda se nunca viu esse estado
        if (bloomFilter.mightContain(estado_hash) && exactSet.count(estado_hash)) {
            continue;
        }
        bloomFilter.insert(estado_hash);
        exactSet.insert(estado_hash);
        exactSetInsertions++;
        if (exactSetInsertions >= exactSetMaxSize) {
            exactSet.clear();
            exactSetInsertions = 0;
        }

        for (auto &mov : movimentos) {
            if (no_atual->mov != -1 && mov.mov == inverso(no_atual->mov)) continue;
            Cubo prox = no_atual->cube;
            mov.acao(prox);
            int g = no_atual->g + mov.custo;
            int h = 0; // Use h=0 para garantir funcionamento, depois melhore a heurística
            int j = g + h;
            auto novo_no = make_shared<A>(prox, no_atual->profund + 1, mov.mov, no_atual, h, g, j);
            // Insere ordenado por j (f = g + h)
            auto it = openList.begin();
            while (it != openList.end() && (*it)->j <= novo_no->j) {
                ++it;
            }
            openList.insert(it, novo_no);
        }
    }
    return {};
}
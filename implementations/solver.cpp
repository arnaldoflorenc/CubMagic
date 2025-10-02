
#include "../class/Solver.h"
#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <stack>
#include <memory>
#include <queue>
#include <unordered_set>

using namespace std;

vector<M> get_moves() {
    return {
        {0, [](Cubo& c){ c.rota_frente(); }, 0},           // F
        {1, [](Cubo& c){ c.rota_frente_anti(); }, 1},   // F' (inverso de F)
        {2, [](Cubo& c){ c.rota_dir(); }, 2},              // R  
        {3, [](Cubo& c){ c.rota_dir_anti(); }, 3},      // R' (inverso de R)
        {4, [](Cubo& c){ c.rota_topo(); }, 4},             // U
        {5, [](Cubo& c){ c.rota_topo_anti(); }, 5}      // U' (inverso de U)
    };
}

int inverso(int mov) {
    switch(mov) {
        case 0: return 1;  // F <-> F'
        case 1: return 0;  
        case 2: return 3;  // R <-> R'
        case 3: return 2;
        case 4: return 5;  // U <-> U'
        case 5: return 4;
    }
    return -1; 
}

vector<int> reconstruir_caminho(shared_ptr<N> no_atual) {
    vector<int> caminho;
    while(no_atual && no_atual->mov != -1) {
        caminho.push_back(no_atual->mov);
        no_atual = no_atual->anterior;
    }
    reverse(caminho.begin(), caminho.end());
    return caminho;
}

vector<int> juntar_caminhos(shared_ptr<N> inicio, shared_ptr<N> fim) {
    vector<int> caminho_inicio, caminho_fim;

    shared_ptr<N> no = inicio;
    while(no && no->mov != -1) {
        caminho_inicio.push_back(no->mov);
        no = no->anterior;
    }
    reverse(caminho_inicio.begin(), caminho_inicio.end());

    no = fim;
    while(no && no->mov != -1) {
        caminho_fim.push_back(no->mov);
        no = no->anterior;
    }
    // inverter a ordem
    reverse(caminho_fim.begin(), caminho_fim.end());
    // inverter os movimentos
    for(auto &m : caminho_fim) {
        m = inverso(m);
    }

    // juntar os caminhos
    caminho_inicio.insert(caminho_inicio.end(), caminho_fim.begin(), caminho_fim.end());
    return caminho_inicio;
}

void func_sucessora(stack<shared_ptr<N>>& pilha, shared_ptr<N> no_atual, const vector<M>& movimentos, unordered_set<size_t>& visitados) {
    for(const auto &mov : movimentos){
        if(no_atual->mov != -1 && mov.mov == inverso(no_atual->mov)) 
            continue;

        auto novo_cubo = make_shared<Cubo>(*no_atual->cube);
        mov.acao(*novo_cubo);

        size_t h = novo_cubo->hash();
        if(visitados.count(h)){
            continue;
        }

        visitados.insert(h);

        auto novo_no = make_shared<N>(N{
            novo_cubo, 
            no_atual->profund + 1, 
            mov.mov, 
            no_atual
        });

        pilha.push(novo_no);
    }
}

vector<int> DFS(Cubo inicio) {
    auto movimentos = get_moves();
    stack<shared_ptr<N>> pilha;
    unordered_set<size_t> visitados;  
    auto estado = make_shared<N>(N{make_shared<Cubo>(inicio), 0, -1, nullptr});

    pilha.push(estado);
    visitados.insert(inicio.hash());

    int estados_explorados = 0;
    int max_estados = 100000; 

    while(!pilha.empty() && estados_explorados < max_estados) {
        auto no_atual = pilha.top();
        pilha.pop();
        estados_explorados++;

        if(no_atual->cube->resolvido()) {
            cout << "SOLUÇÃO ENCONTRADA! Estados explorados: " << estados_explorados 
                 << ", Profundidade: " << no_atual->profund << endl;
            return reconstruir_caminho(no_atual);
        }
        
        if(no_atual->profund < 17) { // Reduz para 8
            func_sucessora(pilha, no_atual, movimentos, visitados);
        }
    }

    return {};
}

void func_sucessora_BFS(queue<shared_ptr<N>>& fila, shared_ptr<N> no_atual, const vector<M>& movimentos, unordered_set<size_t>& visitados) {
    for(const auto &mov : movimentos){
        if(no_atual->mov != -1 && mov.mov == inverso(no_atual->mov)) 
            continue;

        auto novo_cubo = make_shared<Cubo>(*no_atual->cube);
        mov.acao(*novo_cubo);

        size_t h = novo_cubo->hash();
        if(visitados.count(h)){
            continue;
        }

        visitados.insert(h);

        auto novo_no = make_shared<N>(N{
            novo_cubo, 
            no_atual->profund + 1, 
            mov.mov, 
            no_atual
        });

        fila.push(novo_no);
    }
}

vector<int> BFS(Cubo inicio) {
    auto movimentos = get_moves();
    queue<shared_ptr<N>> fila;
    unordered_set<size_t> visitados;  
    auto estado = make_shared<N>(N{make_shared<Cubo>(inicio), 0, -1, nullptr});

    fila.push(estado);
    visitados.insert(inicio.hash());

    int estados_explorados = 0;
    int max_estados = 100000; 

    while(!fila.empty() && estados_explorados < max_estados) {
        auto no_atual = fila.front();
        fila.pop();
        estados_explorados++;

        if(no_atual->cube->resolvido()) {
            cout << "SOLUÇÃO ENCONTRADA! Estados explorados: " << estados_explorados 
                 << ", Profundidade: " << no_atual->profund << endl;
            return reconstruir_caminho(no_atual);
        }
        
        if(no_atual->profund < 14) { // Reduz para 8
            func_sucessora_BFS(fila, no_atual, movimentos, visitados);
        }
    }
    return {};
}
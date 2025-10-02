
#include "../class/Solver.h"
#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <stack>
#include <memory>
#include <queue>
#include <unordered_set>
#include <unordered_map>
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
        {5, [](Cubo& c){c.rota_base(); },1},
        {6, [](Cubo& c){c.rota_frente_anti(); },1},
        {7, [](Cubo& c){c.rota_costa_anti(); },1},
        {8, [](Cubo& c){c.rota_dir_anti(); },1},
        {9, [](Cubo& c){c.rota_esq_anti(); },1},
        {10, [](Cubo& c){c.rota_topo_anti(); },1},
        {11, [](Cubo& c){c.rota_base_anti(); },1}
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

class Heuristica2x2 {
private:
    unordered_map<size_t, int> patternDB;
    const size_t MAX_DB_SIZE = 50000;
    
    // Heurística baseada em posições e orientações dos cantos
    int calculateCornerHeuristic(const Cubo& cube) const {
        int heuristic = 0;
        
        // 1. CANTOS NA POSIÇÃO CORRETA - peso maior
        int correctCorners = 0;
        int orientedCorners = 0;
        
        for (int i = 0; i < 8; i++) {
            if (cube.isCornerCorrect(i)) {
                correctCorners++;
                if (cube.isCornerOriented(i)) {
                    orientedCorners++;
                }
            }
        }
        
        heuristic += (8 - correctCorners) * 3; // Peso maior para posição
        heuristic += (8 - orientedCorners) * 1; // Peso menor para orientação
        
        // 2. BÔNUS para grupos de cantos resolvidos
        // Cantos adjacentes resolvidos indicam faces parcialmente resolvidas
        int bonus = 0;
        
        // Verifica se cantos opostos estão resolvidos (indicador de progresso)
        if (cube.isCornerCorrect(0) && cube.isCornerCorrect(2)) bonus -= 1;
        if (cube.isCornerCorrect(1) && cube.isCornerCorrect(3)) bonus -= 1;
        if (cube.isCornerCorrect(4) && cube.isCornerCorrect(6)) bonus -= 1;
        if (cube.isCornerCorrect(5) && cube.isCornerCorrect(7)) bonus -= 1;
        
        heuristic += bonus;
        
        return max(1, heuristic);
    }
    
    // Heurística alternativa mais agressiva
    int calculateAdvancedHeuristic(const Cubo& cube) const {
        int h1 = calculateCornerHeuristic(cube);
        
        // Heurística adicional baseada na distância de Manhattan
        int manhattan = 0;
        for (int i = 0; i < 8; i++) {
            if (!cube.isCornerCorrect(i)) {
                manhattan += 1; // Distância simplificada
            }
        }
        
        // Combina as duas heurísticas
        return max(h1, manhattan);
    }
    
public:
    int getHeuristic(const Cubo& cube) {
        // Usa o hash do cubo como chave
        size_t patternKey = cube.hash();
        
        // Verifica no lookup table
        auto it = patternDB.find(patternKey);
        if (it != patternDB.end()) {
            return it->second;
        }
        
        // Calcula nova heurística
        int h = calculateAdvancedHeuristic(cube);
        
        // Armazena se há espaço
        if (patternDB.size() < MAX_DB_SIZE) {
            patternDB[patternKey] = h;
        }
        
        return h;
    }
};

Heuristica2x2 he;

int heuristica(Cubo &cube) {
    return he.getHeuristic(cube);
}

vector<int> Aestrela(Cubo inicio) {
    auto movimentos = get_moves();
    
    unordered_map<size_t, int> best_g;
    best_g.reserve(500000);
    
    auto comparador = [](const shared_ptr<AestrelaNo>& a, const shared_ptr<AestrelaNo>& b) {
        return a->f > b->f;
    };
    priority_queue<shared_ptr<AestrelaNo>, vector<shared_ptr<AestrelaNo>>, decltype(comparador)> openList(comparador);
    
    // Inicialização
    int h_inicial = heuristica(inicio);
    auto no_inicial = make_shared<AestrelaNo>(inicio, 0, -1, nullptr, h_inicial, 0);
    best_g[inicio.hash()] = 0;
    openList.push(no_inicial);
    
    int nos_expandidos = 0;
    const int MAX_PROFUNDIDADE = 50;
    
    cout << "A* Iniciando..." << endl;

    while (!openList.empty()) {
        auto no_atual = openList.top();
        openList.pop();
        
        nos_expandidos++;
        
        // Verifica se este nó ainda é o melhor para este estado
        size_t hash_atual = no_atual->cube.hash();
        if (best_g[hash_atual] < no_atual->g) {
            continue;
        }
        
        if (no_atual->cube.resolvido()) {
            cout << "SOLUÇÃO ENCONTRADA!" << endl;
            cout << "Nós expandidos: " << nos_expandidos << ", Movimentos: " << no_atual->g << endl;
            return reconstruir_caminho(static_pointer_cast<No>(no_atual));
        }
        
        if (no_atual->g >= MAX_PROFUNDIDADE) {
            continue;
        }
        
        Cubo cubo_base = no_atual->cube;
        int mov_anterior = no_atual->mov;
        
        for (auto &mov : movimentos) {
            // Evita movimento inverso ao anterior
            if (mov_anterior != -1 && mov.mov == inverso(mov_anterior)) {
                continue;
            }
            
            Cubo prox = cubo_base;
            mov.acao(prox);
            
            int g_novo = no_atual->g + mov.custo;
            
            if (g_novo >= MAX_PROFUNDIDADE) continue;
            
            size_t prox_hash = prox.hash();
            
            // Verifica se encontramos um caminho melhor para este estado
            auto it = best_g.find(prox_hash);
            if (it != best_g.end() && it->second <= g_novo) {
                continue;
            }
            
            // Atualiza melhor custo e adiciona à fila
            best_g[prox_hash] = g_novo;
            int h_novo = heuristica(prox);
            
            auto novo_no = make_shared<AestrelaNo>(
                prox, no_atual->profund + 1, mov.mov, 
                static_pointer_cast<No>(no_atual), h_novo, g_novo
            );
            
            openList.push(novo_no);
        }
        
        // Log periódico para debug
        if (nos_expandidos % 10000 == 0) {
            cout << "Nós expandidos: " << nos_expandidos 
                 << ", Fila: " << openList.size() 
                 << ", Estados únicos: " << best_g.size() << endl;
        }
    }
    
    cout << "Não encontrou solução após " << nos_expandidos << " nós expandidos" << endl;
    return {};
}

// Função auxiliar para converter movimentos (se necessário)
vector<string> converter_movimentos(const vector<int>& caminho) {
    vector<string> movimentos_str;
    vector<string> nomes = {
        "F", "B", "R", "L", "U", "D", 
        "F'", "B'", "R'", "L'", "U'", "D'"
    };
    
    for (int mov : caminho) {
        if (mov >= 0 && mov < nomes.size()) {
            movimentos_str.push_back(nomes[mov]);
        }
    }
    return movimentos_str;
}
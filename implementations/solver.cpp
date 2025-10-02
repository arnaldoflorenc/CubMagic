
#include "../class/Solver.h"
#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <stack>
#include <memory>
#include <queue>
#include <unordered_map>
#include <list>
#include <unordered_set>

using namespace std;

vector<M> get_moves() {
    return {
        {0, [](Cubo& c){ c.rota_frente(); }, 0},           // F
        {1, [](Cubo& c){ c.rota_frente_anti(); }, 1},   // F' (inverso de F)
        {2, [](Cubo& c){ c.rota_dir(); }, 2},              // R  
        {3, [](Cubo& c){ c.rota_dir_anti(); }, 3},      // R' (inverso de R)
        {4, [](Cubo& c){ c.rota_topo(); }, 4},             // U
        {5, [](Cubo& c){ c.rota_topo_anti(); }, 5},
        {6, [](Cubo& c){ c.rota_base(); }, 6},
        {7, [](Cubo& c){ c.rota_base_anti(); }, 7},
        {8, [](Cubo& c){ c.rota_esq(); }, 8},
        {9, [](Cubo& c){ c.rota_esq_anti(); }, 9},
        {10, [](Cubo& c){ c.rota_costa(); }, 10},
        {11, [](Cubo& c){ c.rota_costa_anti(); }, 11} 
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
        case 6: return 7;
        case 7: return 6;
        case 8: return 9;
        case 9: return 8;
        case 10: return 11;
        case 11: return 10;
    }
    return -1; 
}

vector<int> reconstruir_caminho(shared_ptr<N> no_atual) { //recosntrói o caminho da solução
    vector<int> caminho;
    while(no_atual && no_atual->mov != -1) {
        caminho.push_back(no_atual->mov);
        no_atual = no_atual->anterior;
    }
    reverse(caminho.begin(), caminho.end());
    return caminho;
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

// Classe para gerenciar as heurísticas
class Heuristica2x2 {
private:
    unordered_map<size_t, int> patternDB; // Tabela de consulta para padrões
    const size_t MAX_DB_SIZE = 50000; // Limite de tamanho da tabela
    
    // Heurística baseada em posições e orientações dos cantos
    int calculateCornerHeuristic(const Cubo& cube) const { 
        
        int heuristic = 0;
        int correctCorners = 0;
        int orientedCorners = 0;

        // Conta cantos corretos e orientados
        for (int i = 0; i < 8; i++) {
            if (cube.isCornerCorrect(i)) {
                correctCorners++;
                if (cube.isCornerOriented(i)) {
                    orientedCorners++;
                }
            }
        }
        // Combina os fatores na heurística
        heuristic += (8 - correctCorners) * 3; // Peso maior para posição
        heuristic += (8 - orientedCorners) * 1; // Peso menor para orientação
        
        // Bônus se cantos opostos estiverem corretos
        int bonus = 0;
        
        // Verifica se cantos opostos estão resolvidos
        if (cube.isCornerCorrect(0) && cube.isCornerCorrect(2)) bonus -= 1;
        if (cube.isCornerCorrect(1) && cube.isCornerCorrect(3)) bonus -= 1;
        if (cube.isCornerCorrect(4) && cube.isCornerCorrect(6)) bonus -= 1;
        if (cube.isCornerCorrect(5) && cube.isCornerCorrect(7)) bonus -= 1;
        
        heuristic += bonus;
        
        return max(1, heuristic);
    }
    
    // Cálculo da Heurística 
    int calculateAdvancedHeuristic(const Cubo& cube) const {
        int h1 = calculateCornerHeuristic(cube);
        
        // Heurística adicional baseada na distância de Manhattan
        int manhattan = 0;
        for (int i = 0; i < 8; i++) {
            if (!cube.isCornerCorrect(i)) {
                manhattan += 1; // Cada canto fora do lugar adiciona 1
            }
        }
        
        // Combina as duas heurísticas
        return max(h1, manhattan);
    }
    
public:
    // Função para obter a heurística
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
// Função obter a heurística
int heuristica(Cubo &cube) {
    return he.getHeuristic(cube);
}
// Implementação do algoritmo A*
vector<int> Aestrela(Cubo inicio) {
    auto movimentos = get_moves();
    // Mapa para armazenar o melhor custo g para cada estado
    unordered_map<size_t, int> best_g;
    best_g.reserve(500000);

    // Comparador para a priority_queue
    auto comparador = [](const shared_ptr<AestrelaNo>& a, const shared_ptr<AestrelaNo>& b) {
        return a->f > b->f;
    };
    priority_queue<shared_ptr<AestrelaNo>, vector<shared_ptr<AestrelaNo>>, decltype(comparador)> openList(comparador);
    
    // Inicialização
    int h_inicial = heuristica(inicio);
    auto no_inicial = make_shared<AestrelaNo>(make_shared<Cubo>(inicio), 0, -1, nullptr, h_inicial, 0);
    best_g[inicio.hash()] = 0;
    openList.push(no_inicial);

    // Contadores e limites
    int nos_expandidos = 0;
    const int MAX_PROFUNDIDADE = 17;
    
    cout << "A* Iniciando..." << endl;
    // Loop principal do A*
    while (!openList.empty()) {
        auto no_atual = openList.top();
        openList.pop();
        
        nos_expandidos++;
        
        // Verifica se este nó ainda é o melhor para este estado
        size_t hash_atual = no_atual->cube->hash();
        if (best_g[hash_atual] < no_atual->g) {
            continue;
        }

        // Verifica se é solução
        if (no_atual->cube->resolvido()) {
            cout << "SOLUÇÃO ENCONTRADA!" << endl;
            cout << "Nós expandidos: " << nos_expandidos << ", Movimentos: " << no_atual->g << endl;
            return reconstruir_caminho(static_pointer_cast<No>(no_atual));
        }
        // Limita profundidade
        if (no_atual->g >= MAX_PROFUNDIDADE) {
            continue;
        }

        // Expansão dos nós
        Cubo cubo_base = *(no_atual->cube);
        int mov_anterior = no_atual->mov;
        
        // Gera nós filhos
        for (auto &mov : movimentos) {
            // Evita movimento inverso ao anterior
            if (mov_anterior != -1 && mov.mov == inverso(mov_anterior)) {
                continue;
            }
            
            Cubo prox = cubo_base;
            mov.acao(prox);
            // Calcula novo custo g
            int g_novo = no_atual->g + mov.custo;
            
            // Limita profundidade
            if (g_novo >= MAX_PROFUNDIDADE) continue;
            
            size_t prox_hash = prox.hash();
            
            // Verifica se encontrou um caminho melhor para este estado
            auto it = best_g.find(prox_hash);
            if (it != best_g.end() && it->second <= g_novo) {
                continue;
            }
            
            // Atualiza melhor custo e adiciona à fila de prioridade
            best_g[prox_hash] = g_novo;
            int h_novo = heuristica(prox);
            
            // Cria novo nó A*
            auto novo_no = make_shared<AestrelaNo>(
                make_shared<Cubo>(prox), no_atual->profund + 1, mov.mov, 
                static_pointer_cast<No>(no_atual), h_novo, g_novo
            );

            openList.push(novo_no);
        }
        
        // Debug
        if (nos_expandidos % 10000 == 0) {
            cout << "Nós expandidos: " << nos_expandidos 
                 << ", Fila: " << openList.size() 
                 << ", Estados únicos: " << best_g.size() << endl;
        }
    }
    
    // Nenhuma solução encontrada
    cout << "Não encontrou solução após " << nos_expandidos << " nós expandidos" << endl;
    return {};
}
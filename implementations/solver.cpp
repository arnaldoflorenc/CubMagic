
#include "../class/Solver.h"
#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <stack>
#include <memory>
#include <queue>
#include <unordered_set>
#include "../class/bloom.h"

using namespace std;

vector<M> get_moves() {
    return {
        {0, [](Cubo& c){ c.rota_frente(); }, 0}, // frente
        {1, [](Cubo& c){ c.rota_costa(); }, 1},  // costa
        {2, [](Cubo& c){ c.rota_dir(); }, 2},    // direita
        {3, [](Cubo& c){ c.rota_esq(); }, 3},    // esquerda
        {4, [](Cubo& c){ c.rota_topo(); }, 4},   // topo
        {5, [](Cubo& c){ c.rota_base(); }, 5}    // base
    };
}

int inverso(int mov) {
    switch(mov) {
        case 0: return 1; // frente <-> costa
        case 1: return 0;
        case 2: return 3; // direita <-> esquerda
        case 3: return 2;
        case 4: return 5; // topo <-> base
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

vector<int> DFS(Cubo inicio) {
    auto movimentos = get_moves();

    bloom bloomFilter(16'000'000, 7);
    unordered_set<size_t> exactSet;

    stack<shared_ptr<N>> pilha;
    auto no_inicial = make_shared<N>(N{make_shared<Cubo>(inicio), 0, -1, nullptr});
    pilha.push(no_inicial);

    bloomFilter.insert(no_inicial->cube->hash());
    exactSet.insert(no_inicial->cube->hash());

    while(!pilha.empty()) {
        auto no_atual = pilha.top(); 
        pilha.pop();

        if(no_atual->cube->resolvido()) {
            return reconstruir_caminho(no_atual);
        }

        for(auto &mov : movimentos) {
            if(no_atual->mov != -1 && mov.mov == inverso(no_atual->mov)) continue;

            auto prox = make_shared<Cubo>(*no_atual->cube);
            prox->aplica_movimento(mov.mov);

            size_t prox_hash = prox->hash();
            if(exactSet.count(prox_hash) || bloomFilter.mightContain(prox_hash)) continue;

            exactSet.insert(prox_hash);
            bloomFilter.insert(prox_hash);

            pilha.push(make_shared<N>(N{prox, no_atual->profund + 1, mov.mov, no_atual}));
        }
    }
    return {};
}

vector<int> BFS(Cubo inicio, int limite = 20) {
    auto movimentos = get_moves();

    // Estado inicial e final
    auto no_inicial = make_shared<N>(N{make_shared<Cubo>(inicio), 0, -1, nullptr});
    Cubo cubo_resolvido;
    auto no_final = make_shared<N>(N{make_shared<Cubo>(cubo_resolvido), 0, -1, nullptr});

    // Verificar se já está resolvido
    if(inicio.resolvido()) {
        cout << "Já está resolvido!" << endl;
        return {};
    }

    queue<shared_ptr<N>> fila_inicio, fila_fim;
    unordered_map<size_t, shared_ptr<N>> visitados_inicio, visitados_fim;

    fila_inicio.push(no_inicial);
    fila_fim.push(no_final);

    visitados_inicio[no_inicial->cube->hash()] = no_inicial;
    visitados_fim[no_final->cube->hash()] = no_final;

    auto expandir_nivel = [&](queue<shared_ptr<N>>& fila, 
                             unordered_map<size_t, shared_ptr<N>>& meus_visitados,
                             unordered_map<size_t, shared_ptr<N>>& outros_visitados,
                             bool expandindo_do_inicio) -> vector<int> {
        
        int tamanho_nivel = fila.size();
        if (tamanho_nivel == 0) return {};
        
        for(int i = 0; i < tamanho_nivel; i++) {
            auto no_atual = fila.front();
            fila.pop();

            // Verificar se encontramos solução direta
            if(no_atual->cube->resolvido()) {
                return reconstruir_caminho(no_atual);
            }

            // Verificar limite de profundidade
            if(no_atual->profund >= limite) {
                continue;
            }

            for(auto &mov : movimentos) {
                // Pular movimento inverso ao anterior
                if(no_atual->mov != -1 && mov.mov == inverso(no_atual->mov)) 
                    continue;

                auto prox_cubo = make_shared<Cubo>(*no_atual->cube);
                mov.acao(*prox_cubo);

                size_t h = prox_cubo->hash();
                
                // Se já visitamos este estado, pular
                if(meus_visitados.count(h)) 
                    continue;

                auto novo_no = make_shared<N>(N{prox_cubo, no_atual->profund + 1, mov.mov, no_atual});
                meus_visitados[h] = novo_no;

                // Verificar colisão com o outro BFS
                auto it = outros_visitados.find(h);
                if(it != outros_visitados.end()) {

                    // Verificar se o estado é válido
                    Cubo teste = *novo_no->cube;
                    
                    if(expandindo_do_inicio) {
                        return juntar_caminhos(novo_no, it->second);
                    } else {
                        return juntar_caminhos(it->second, novo_no);
                    }
                }

                fila.push(novo_no);
            }
        }
        
        return {};
    };

    int iteracoes = 0;
    int max_iteracoes = 1000;

    while(iteracoes < max_iteracoes) {
        iteracoes++;

        // Se ambas as filas estão vazias, parar
        if(fila_inicio.empty() && fila_fim.empty()) {
            break;
        }

        // Expansão do início para o fim
        if(!fila_inicio.empty()) {
            vector<int> sol = expandir_nivel(fila_inicio, visitados_inicio, visitados_fim, true);
            if(!sol.empty()) {
                return sol;
            }
        }

        // Expansão do fim para o início  
        if(!fila_fim.empty()) {
            vector<int> sol = expandir_nivel(fila_fim, visitados_fim, visitados_inicio, false);
            if(!sol.empty()) {
                return sol;
            }
        }

    }
    return DFS(inicio);
}
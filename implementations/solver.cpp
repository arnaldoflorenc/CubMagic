#include "../class/Solver.h"
#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <stack>
#include <queue>
#include <unordered_set>

vector<M> get_moves(){
    return{
        {"F", [](Cubo& c){c.rota_frente(); }, 1},
        {"C", [](Cubo& c){c.rota_costa(); },1},
        {"D", [](Cubo& c){c.rota_dir(); },1},
        {"E", [](Cubo& c){c.rota_esq(); },1},
        {"Y", [](Cubo& c){c.rota_cub_cima(); },1},
        {"X", [](Cubo& c){c.rota_cub_dir(); },1}
    };
}

vector<string> DFS(Cubo inicio){ //busca uma solução enquanto n tiver chego no limite estabelecido
    auto movimentos = get_moves();
    unordered_set<string> visitados;

    stack<N> pilha;
    pilha.push({inicio, 0, {}});

    while(!pilha.empty()){
        N estado_atual =  pilha.top();
        pilha.pop();
        
        
        if(estado_atual.cube.resolvido()){
            cout<<"Resolvido"<<endl;
            return estado_atual.caminho;
        }
        cout<<"nao resolvido"<<endl;

        if(estado_atual.profund >= 14){
            continue;
        }
        
        string estado = estado_atual.cube.transfString();
        if(visitados.count(estado)){
            continue;
        }     

        visitados.insert(estado);

        for(auto &mov : movimentos){
            Cubo prox = estado_atual.cube;
            mov.acao(prox);

            vector<string> n_caminho = estado_atual.caminho;
            n_caminho.push_back(mov.mov);
            pilha.push(N{prox, estado_atual.profund + mov.custo, n_caminho});
        }
    }    
    return {};
}

vector<string> BFS(Cubo inicio){
    auto movimentos = get_moves();
    unordered_set<string> visitados;

    queue<N> gila;
    gila.push({inicio, 0, {}});
    while(!gila.empty()){
        N no_atual = gila.front();
        gila.pop();
        if(no_atual.cube.resolvido()){
            return no_atual.caminho;
        }

        if(no_atual.profund >= 13){
            continue;
        }

        string no = no_atual.cube.transfString();
        if(visitados.count(no)){
            continue;
        }
        visitados.insert(no);

        for(auto &mov : movimentos){
            Cubo prox = no_atual.cube;
            mov.acao(prox);

            vector<string> n_caminho = no_atual.caminho;
            n_caminho.push_back(mov.mov);
            gila.push(N{prox, no_atual.profund + mov.custo, n_caminho});
        }
    }
    return {};
}
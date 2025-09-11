#include "../class/Solver.h"
#include <vector>
#include <string>
#include <functional>
#include <stack>
#include <unordered_set>

vector<pair<string, function<void(Cubo&)>>> get_moves(){
    return{
        {"F", [](Cubo& c){c.rota_frente();}},
        {"C", [](Cubo& c){c.rota_costa();}},
        {"D", [](Cubo& c){c.rota_dir();}},
        {"E", [](Cubo& c){c.rota_esq();}},
        {"Y", [](Cubo& c){c.rota_cub_cima();}},
        {"X", [](Cubo& c){c.rota_cub_dir();}}
    };
}

vector<string> cubo_resolve(Cubo inicio){ //busca uma solução enquanto n tiver chego no limite estabelecido
    auto movimentos = get_moves();
    unordered_set<string> visitados;

    stack<E> pilha;
    pilha.push({inicio, 0, {}});

    while(!pilha.empty()){
        E estado_atual =  pilha.top();
        pilha.pop();
        

        if(estado_atual.cube.resolvido()){
            return estado_atual.caminho;
        }

        if(estado_atual.profund == 14){
            continue;
        }
        
        string estado = estado_atual.cube.transfString();
        if(visitados.count(estado)){
            continue;
        }     
        visitados.insert(estado);

        for(auto &mov : movimentos){
            Cubo prox = estado_atual.cube;
            mov.second(prox);

            vector<string> n_caminho = estado_atual.caminho;
            n_caminho.push_back(mov.first);

            pilha.push(E{prox, estado_atual.profund + 1, n_caminho});
        }
    }    
    return {};
}
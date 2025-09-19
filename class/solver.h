#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <string>
#include <functional>
#include <stack>
#include <unordered_set>
#include "Cubo.h"

typedef struct No { 
    Cubo cube; 
    int profund; 
    vector<string> caminho; 
}N; 

typedef struct Movimetos{
    string mov;
    function<void(Cubo&)> acao;
    int custo;
}M;

vector<M> get_moves(); 
vector<string> DFS(Cubo inicio);
vector<string> BFS(Cubo inicio);

#endif
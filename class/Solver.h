#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <string>
#include <functional>
#include <stack>
#include <unordered_set>
#include "Cubo.h"

typedef struct Estado { 
    Cubo cube; 
    int profund; 
    vector<string> caminho; 
}E; 

typedef struct Movimetos{
    string mov;
    function<void(Cubo&)> acao;
    int custo;
}M;

vector<M> get_moves(); 
vector<string> cubo_resolve(Cubo inicio);

#endif
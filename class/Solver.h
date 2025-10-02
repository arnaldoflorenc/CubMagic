#ifndef SOLVER_H
#define SOLVER_H


#include <vector>
#include <string>
#include <functional>
#include <stack>
#include <queue>
#include <unordered_set>
#include <memory>
#include "Cubo.h"
#include "bloom.h"

using namespace std;

typedef struct No { 
    shared_ptr<Cubo> cube; 
    int profund; 
    int mov;
    shared_ptr<No> anterior; 
}N;

typedef struct Movimetos{
    int mov;
    void (*acao)(Cubo&);
    int custo;
}M;


vector<M> get_moves();
int inverso(int mov);
vector<int> DFS(Cubo inicio);
vector<int> BFS(Cubo inicio, int limite);
vector<int> reconstruir_caminho(shared_ptr<N> no_final);
vector<string> converter_movimentos(const vector<int>& caminho);

#endif
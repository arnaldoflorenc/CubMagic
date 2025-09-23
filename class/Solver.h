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
    Cubo cube; 
    int profund; 
    int mov;
    shared_ptr<No> anterior; 
}N; 

typedef struct Movimetos{
    int mov;
    function<void(Cubo&)> acao;
    int custo;
}M;


vector<M> get_moves();
int inverso(int mov);
vector<int> DFS(Cubo inicio, size_t bloomSize, int bloomHashes);
vector<int> BFS(Cubo inicio, size_t bloomSize, int bloomHashes);
vector<int> reconstruir_caminho(shared_ptr<N> no_final);
vector<string> converter_movimentos(const vector<int>& caminho);

#endif
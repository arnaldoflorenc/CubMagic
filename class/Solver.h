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
void func_sucessora(stack<shared_ptr<N>>& pilha, shared_ptr<N> no_atual, const vector<M>& movimentos, unordered_set<size_t>& visitados);
vector<int> DFS(Cubo inicio);
void func_sucessora_BFS(stack<shared_ptr<N>>& pilha, shared_ptr<N> no_atual, const vector<M>& movimentos, unordered_set<size_t>& visitados);
vector<int> BFS(Cubo inicio);
vector<int> reconstruir_caminho(shared_ptr<N> no_final);
vector<string> converter_movimentos(const vector<int>& caminho);

#endif
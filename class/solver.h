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

     No(shared_ptr<Cubo> c, int p, int m, shared_ptr<No> ant) 
        : cube(c), profund(p), mov(m), anterior(ant) {}
}N;

typedef struct Movimetos{
    int mov;
    void (*acao)(Cubo&);
    int custo;
}M;

struct AestrelaNo : public No {
   int h, g, f;

    AestrelaNo(shared_ptr<Cubo> cube, int profund, int mov, shared_ptr<No> anterior, int h_val, int g_val)
        : No(cube, profund, mov, anterior), h(h_val), g(g_val), f(g_val + h_val) {}

    bool operator()(const shared_ptr<AestrelaNo>& a, const shared_ptr<AestrelaNo>& b) {
        return a->f > b->f;
    }
};

using N = No;
using A = AestrelaNo;

vector<M> get_moves();
int inverso(int mov);
void func_sucessora(stack<shared_ptr<N>>& pilha, shared_ptr<N> no_atual, const vector<M>& movimentos, unordered_set<size_t>& visitados);
vector<int> DFS(Cubo inicio);
void func_sucessora_BFS(stack<shared_ptr<N>>& pilha, shared_ptr<N> no_atual, const vector<M>& movimentos, unordered_set<size_t>& visitados);
vector<int> BFS(Cubo inicio);
vector<int> Aestrela(Cubo inicio);
vector<int> reconstruir_caminho(shared_ptr<N> no_final);
vector<string> converter_movimentos(const vector<int>& caminho);

#endif
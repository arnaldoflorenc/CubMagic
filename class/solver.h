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

struct No { 
    Cubo cube; 
    int profund; 
    int mov;
    shared_ptr<No> anterior; 
    virtual ~No() = default;
    No(const Cubo& cube, int profund, int mov, shared_ptr<No> anterior)
        : cube(cube), profund(profund), mov(mov), anterior(anterior) {}
}; 

typedef struct Movimetos{
    int mov;
    function<void(Cubo&)> acao;
    int custo;
}M;

struct AestrelaNo : public No {
    int h, g, f;
    
    AestrelaNo(const Cubo& cube, int profund, int mov, shared_ptr<No> anterior, int h_val, int g_val)
        : No(cube, profund, mov, anterior), h(h_val), g(g_val), f(g_val + h_val) {}

    bool operator()(const shared_ptr<AestrelaNo>& a, const shared_ptr<AestrelaNo>& b) {
        return a->f > b->f;
    }

};


using N = No;
using A = AestrelaNo;

vector<M> get_moves();
int inverso(int mov);
vector<int> DFS(Cubo inicio, size_t bloomSize, int bloomHashes);
vector<int> BFS(Cubo inicio, size_t bloomSize, int bloomHashes);
vector<int> Aestrela(Cubo inicio);
vector<int> reconstruir_caminho(shared_ptr<N> no_final);
vector<string> converter_movimentos(const vector<int>& caminho);

#endif
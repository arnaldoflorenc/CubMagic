#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <string>
#include <functional>
#include <utility>
#include "Cubo.h"
using namespace std;

typedef struct Estado {
    Cubo cube;
    int profund;
    vector<string> caminho;
}E;

vector<pair<string, function<void(Cubo&)>>> get_moves();
vector<string> cubo_resolve(Cubo inicio);

#endif
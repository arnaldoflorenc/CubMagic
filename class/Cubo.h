#ifndef CUBO_H
#define CUBO_H

#include <array>
#include <string>
#include <functional>  // Para std::hash

using namespace std;

class Cubo {
private: 
    array<array<char,4>,6> face;
    int custo_mov;

public:
    Cubo();

    size_t hash() const;

    void printar() const;

    bool resolvido() const;

    string transfString();

    string transfStringCanonical() const;

    void rota_frente();
    void rota_costa();
    void rota_dir();
    void rota_esq();
    void rota_cub_dir();
    void rota_cub_cima();
    void rota_topo();
    void rota_base();
    void embaralha();

    const array<char,4>& getFace(int faceIndex) const {
        return face[faceIndex];
    }
    
    char getFaceColor(int faceIndex, int position) const {
        return face[faceIndex][position];
    }
    
    char getExpectedColor(int faceIndex) const {
        static const char expected[6] = {'W','R','Y','O','B','G'};
        return expected[faceIndex];
    }
};

#endif
#ifndef CUBO_H
#define CUBO_H

#include <array>
#include <string>
#include <functional>  // Para std::hash

using namespace std;

class Cubo {
private: 
    array<int, 8> posicoes;     // Posição dos cantos (0-7)
    array<int, 8> orientacoes;  // Orientação dos cantos (0-2)
    int custo_mov;

public:
    Cubo();

    size_t hash() const;

    void printar() const;

    bool resolvido() const;

    void rota_frente();
    void rota_costa();
    void rota_dir();
    void rota_esq();
    void rota_topo();
    void rota_base();
    void embaralha();
    void rota_frente_anti();
    void rota_costa_anti();
    void rota_dir_anti();
    void rota_esq_anti();
    void rota_topo_anti();
    void rota_base_anti();

    int countCorrectCorners() const;
    int countOrientedCorners() const;
    int countSolvedFaces() const;

    bool isCornerCorrect(int corner) const;
    bool isCornerOriented(int corner) const;

    void printar_bonito() const;
    array<array<char, 4>, 6> para_faces() const;
};

#endif
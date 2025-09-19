#ifndef CUBO_H
#define CUBO_H

#include <array>
#include <string>
using namespace std;

class Cubo{
    private: 
        int custo_mov;

    public:
        Cubo();

        array<array<char,4>,6> face;

        void printar() const;

        bool resolvido();

        string transfString();

        void rota_frente();

        void rota_costa();

        void rota_dir();

        void rota_esq();

        void rota_cub_dir();

        void rota_cub_cima();

        void embaralha();
};
#endif
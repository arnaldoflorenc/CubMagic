#ifndef CUBO_H
#define CUBO_H

#include <array>
#include <string>
using namespace std;

class Cubo{
    private: 
        array<array<char,4>,6> face;
        size_t hash_atual;
        int custo_mov;

    public:
        Cubo();

        void atualiza_hash();

        void aplica_movimento(int mov);

        size_t hash() const;

        void printar() const;

        bool resolvido();

        string transfString();

        string transfStringCanonical();

        void rota_frente();

        void rota_costa();

        void rota_dir();

        void rota_esq();

        void rota_cub_dir();

        void rota_cub_cima();

        void rota_topo();
        
        void rota_base();

        void embaralha();
};
#endif
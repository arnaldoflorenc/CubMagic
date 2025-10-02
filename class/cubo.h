#ifndef CUBO_H
#define CUBO_H

#include <array>
#include <string>
#include <functional>  // Para std::hash
using namespace std;

class Cubo{
    private: 
        array<array<char,4>,6> face;
        size_t hash_atual;
        int custo_mov;

    public:
        Cubo();

        void atualiza_hash();

        char getFaceChar(int face_index, int idx) const;

        char getFaceColor(int face, int pos) const;

        void aplica_movimento(int mov);

        size_t hash() const;

        void printar() const;

        bool resolvido() const;

        // debug / validação
        bool validar_stickers() const;          // true se cada cor aparece exatamente 4x
        bool igual(const Cubo& outro) const;    // compara faces
        void printStickerCounts() const;        // imprime contagem por cor

        string transfString() const;

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

        char getExpectedColor(int faceIndex) const {
            static const char expected[6] = {'W','R','Y','O','B','G'};
            return expected[faceIndex];
        }

        const array<char,4>& getFace(int faceIndex) const {
            return face[faceIndex];
        }
};
#endif
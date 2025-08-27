#include <iostream>
#include <array>
using namespace std;

class Cubo{
    public: 
        array<array<char,4>,6> face;

    public:
        Cubo(){
            char cor[6] = {'W','R','Y','O','B','G'};
            for(int i = 0; i < 6; i++){
                for(int j = 0; j < 4; j++){
                    face[i][j] = cor[i];
                }
            }
        }

        void printar() const {
            for (int i = 0; i < 6; i++) {
                cout << "Face " << i << ": ";
                for (int j = 0; j < 4; j++) {
                    cout << face[i][j] << " ";
                }
                cout << endl;
            }
        }

        void rota_frente(){
            char aux = face[0][0];
           //face
           face[0][0] = face[0][3];
           face[0][3] = face[0][2];
           face[0][2] = face[0][1];
           face[0][1] = aux;
           //laterais
            array<char,2> face_aux;
            face_aux[0] = face[3][2];
            face_aux[1] = face[3][1];
            //-baixo para esquerda
            face[3][1] = face[5][3];
            face[3][2] = face[5][2];
            //-dir para baixo
            face[5][2] = face[1][0];
            face[5][3] = face[1][3];
            //-topo para dir
            face[1][0] = face[4][3];
            face[1][3] = face[4][2];
            //-esq para topo
            face[4][3] = face_aux[0];
            face[4][2] = face_aux[1];
        }

        void rota_costa(){
            char aux = face[2][0];
           //face
           face[2][0] = face[2][3];
           face[2][3] = face[2][2];
           face[2][2] = face[2][1];
           face[2][1] = aux;
           //laterais
            array<char,2> face_aux;
            face_aux[0] = face[3][0];
            face_aux[1] = face[3][3];
            //-baixo para esquerda
            face[3][1] = face[5][3];
            face[3][2] = face[5][2];
            //-dir para baixo
            face[5][2] = face[1][0];
            face[5][3] = face[1][3];
            //-topo para dir
            face[1][0] = face[4][3];
            face[1][3] = face[4][2];
            //-esq para topo
            face[4][3] = face_aux[0];
            face[4][2] = face_aux[1];
        }
};

int main(){
    Cubo cub;
    // face 0= branco face 1= vermelho face 2=amarelo face 3=laranja face 4=azul face 5=verde
    cout<<"Cubo:\n";
    cub.rota_frente();
    cub.printar();

    return 0;
}
#include <iostream>
#include "../class/Cubo.h"
using namespace std;

Cubo::Cubo(){
        char cor[6] = {'W','R','Y','O','B','G'};
        for(int i = 0; i < 6; i++){
            for(int j = 0; j < 4; j++){
                face[i][j] = cor[i];
            }
        }
}

void Cubo::printar() const {
    for (int i = 0; i < 6; i++) {
        cout << "Face " << i << ": ";
        for (int j = 0; j < 4; j++) {
            cout << face[i][j] << " ";
        }
        cout << endl;
    }
    cout<<endl;
}

bool Cubo::resolvido(){
    for(int i = 0; i < 6; i++){
        char cor = face[i][0];
        for(int j = 0; j < 4; j++){
            if(face[i][j] != cor){
                return false;
            }
        }
    }
    return true;
}

string Cubo::transfString(){
    string transform;
    transform.reserve(24);
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 4; j++){
            transform.push_back(face[i][j]);
        }
    }
    return transform;
}

void Cubo::rota_frente(){
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

void Cubo::rota_costa(){
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
    face[3][0] = face[5][0];
    face[3][3] = face[5][1];
    //-dir para baixo
    face[5][1] = face[1][1];
    face[5][0] = face[1][2];
    //-topo para dir
    face[1][1] = face[4][0];
    face[1][2] = face[4][1];
    //-esq para topo
    face[4][1] = face_aux[0];
    face[4][0] = face_aux[1];
}

void Cubo::rota_dir(){
    char aux = face[1][0];
    //face
    face[1][0] = face[1][3];
    face[1][3] = face[1][2];
    face[1][2] = face[1][1];
    face[1][1] = aux;
    //laterais
    array<char,2> face_aux;
    face_aux[0] = face[0][1];
    face_aux[1] = face[0][2];
    //-baixo para frente
    face[0][2] = face[5][1];
    face[0][1] = face[5][2];
    //-fundo para baixo
    face[5][1] = face[2][0];
    face[5][2] = face[2][3];
    //-topo para fundo
    face[2][3] = face[4][1];
    face[2][0] = face[4][2];
    //-frente para topo
    face[4][1] = face_aux[0];
    face[4][2] = face_aux[1];
}

void Cubo::rota_esq(){
    char aux = face[3][0];
    //face
    face[3][0] = face[3][1];
    face[3][1] = face[3][2];
    face[3][2] = face[3][3];
    face[3][3] = aux;
    //laterais
    array<char,2> face_aux;
    face_aux[0] = face[0][0];
    face_aux[1] = face[0][3];
    //-baixo para frente
    face[0][3] = face[5][0];
    face[0][0] = face[5][3];
    //-fundo para baixo
    face[5][0] = face[2][1];
    face[5][3] = face[2][2];
    //-topo para fundo
    face[2][2] = face[4][0];
    face[2][1] = face[4][3];
    //-frente para topo
    face[4][0] = face_aux[0];
    face[4][3] = face_aux[1];
}

void Cubo::rota_cub_dir(){
    rota_costa();
    rota_frente();
}

void Cubo::rota_cub_cima(){
    rota_dir();
    rota_esq();
}

void Cubo::embaralha(){
    rota_costa();
    rota_dir();
}
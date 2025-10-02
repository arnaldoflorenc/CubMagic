#include <iostream>
#include <algorithm>
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

size_t Cubo::hash() const{
    size_t resultado = 0; 
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 4; j++){
            resultado ^= std::hash<char>{}(face[i][j]) + 0x9e3779b9 + (resultado << 6) + (resultado >> 2);
        }
    }
    return resultado;
}

string Cubo::transfStringCanonical() const {
    string result;
    result.reserve(24); // 6 faces * 4 elementos = 24 caracteres
    
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 4; j++) {
            result += face[i][j];
        }
    }
    return result;
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

bool Cubo::resolvido() const {
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

void Cubo::rota_frente(){
    char aux = face[0][0];
    //face
    face[0][0] = face[0][2];
    face[0][2] = face[0][3];
    face[0][3] = face[0][1];
    face[0][1] = aux;
    //laterais
    char temp1 = face[4][1];
    char temp2 = face[4][2];
    //-esquerda para cima
    face[4][1] = face[3][2];
    face[4][2] = face[3][0];
    //-baixo para esq
    face[3][0] = face[5][2];
    face[3][2] = face[5][1];
    //-direita para baixo
    face[5][1] = face[1][3];
    face[5][2] = face[1][0];
    //-esq para topo
    face[1][0] = temp1;
    face[1][3] = temp2;
}

void Cubo::rota_costa() {
    char aux = face[2][0];
    face[2][0] = face[2][2];
    face[2][2] = face[2][3];
    face[2][3] = face[2][1];
    face[2][1] = aux;
    
    //topo
    char temp1 = face[4][0];
    char temp2 = face[4][3];
    
    //direita para topo
    face[4][0] = face[1][1];
    face[4][3] = face[1][2];
    
    //baixo para dir
    face[1][1] = face[5][3];
    face[1][2] = face[5][0];
    
    //esq para baixo
    face[5][0] = face[3][3];
    face[5][3] = face[3][1];
    
    //topo para esq
    face[3][1] = temp1;
    face[3][3] = temp2;
}

void Cubo::rota_dir() {
    char aux = face[1][0];
    face[1][0] = face[1][2];
    face[1][2] = face[1][3];
    face[1][3] = face[1][1];
    face[1][1] = aux;
    
    //dir
    char temp1 = face[0][1];
    char temp2 = face[0][2];
    
    //topo para frente 
    face[0][1] = face[4][1];
    face[0][2] = face[4][2];
    
    //costa para topo
    face[4][1] = face[2][0];
    face[4][2] = face[2][3];
    
    //baixo para costa
    face[2][0] = face[5][1];
    face[2][3] = face[5][2];
    
    //frente para baixo
    face[5][1] = temp1;
    face[5][2] = temp2;
}

void Cubo::rota_esq() {
    char aux = face[3][0];
    face[3][0] = face[3][2];
    face[3][2] = face[3][3];
    face[3][3] = face[3][1];
    face[3][1] = aux;
    
    //salva esq
    char temp1 = face[0][0];
    char temp2 = face[0][3];
    
    //baixo para frente
    face[0][0] = face[5][0];
    face[0][3] = face[5][3];
    
    //costas para baixo
    face[5][0] = face[2][1];
    face[5][3] = face[2][2];
    
    //topo par costas
    face[2][1] = face[4][0];
    face[2][2] = face[4][3];
    
    //frente para topo
    face[4][0] = temp1;
    face[4][3] = temp2;
}

void Cubo::rota_topo() {
    char aux = face[4][0];
    face[4][0] = face[4][2];
    face[4][2] = face[4][3];
    face[4][3] = face[4][1];
    face[4][1] = aux;
    
    //topo
    char temp1 = face[0][0];
    char temp2 = face[0][1];
    
    //dir para frente
    face[0][0] = face[1][0];
    face[0][1] = face[1][1];
    
    //costa para dir
    face[1][0] = face[2][0];
    face[1][1] = face[2][1];
    
    //esq para costa
    face[2][0] = face[3][0];
    face[2][1] = face[3][1];
    
    //frente para esq
    face[3][0] = temp1;
    face[3][1] = temp2;
}

void Cubo::rota_base() {
    char aux = face[5][0];
    face[5][0] = face[5][2];
    face[5][2] = face[5][3];
    face[5][3] = face[5][1];
    face[5][1] = aux;
    
    //frente
    char temp1 = face[0][2];
    char temp2 = face[0][3];
    
    //esq para frente
    face[0][2] = face[3][2];
    face[0][3] = face[3][3];
    
    //tras para esq
    face[3][2] = face[2][2];
    face[3][3] = face[2][3];
    
    //direita para tras
    face[2][2] = face[1][2];
    face[2][3] = face[1][3];
    
    // frente para dir
    face[1][2] = temp1;
    face[1][3] = temp2;
}

void Cubo::embaralha(){
    rota_frente();
    rota_dir();
    rota_topo();
    rota_costa();
}   

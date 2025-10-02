#include <iostream>
#include <algorithm>
#include <string>
#include "../class/Cubo.h"
using namespace std;

Cubo::Cubo(){
        char cor[6] = {'W','R','Y','O','B','G'};
        for(int i = 0; i < 6; i++){
            for(int j = 0; j < 4; j++){
                face[i][j] = cor[i];
            }
        }
        atualiza_hash();
}

size_t Cubo::hash() const{
    return hash_atual;
}


void Cubo::atualiza_hash() {
    size_t h = 1469598103934665603ULL; // offset basis (64-bit)
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 4; j++) {
            h ^= static_cast<size_t>(face[i][j]);
            h *= 1099511628211ULL; // FNV prime (64-bit)
        }
    }
    hash_atual = h;
}

// Retorna todas as cores do cubo em uma string de 24 caracteres
string Cubo::transfString() const {
    string s = "";
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 4; j++){
            s += face[i][j];
        }
    }
    return s;
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

char Cubo::getFaceChar(int face_index, int idx) const {
    return face[face_index][idx];
}

char Cubo::getFaceColor(int face, int pos) const {
    return this->face[face][pos];
}


void Cubo::aplica_movimento(int mov) {
    switch(mov) {
        case 0: rota_frente(); break;
        case 1: rota_costa(); break;
        case 2: rota_dir(); break;
        case 3: rota_esq(); break;
        case 4: rota_topo(); break;
        case 5: rota_base(); break;
    }
    atualiza_hash();
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

bool Cubo::resolvido() const{
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

static int colorIndex(char c){
    switch(c){
        case 'W': return 0;
        case 'R': return 1;
        case 'Y': return 2;
        case 'O': return 3;
        case 'B': return 4;
        case 'G': return 5;
        default: return -1;
    }
}

bool Cubo::validar_stickers() const {
    int counts[6] = {0,0,0,0,0,0};
    for(int f=0; f<6; ++f){
        for(int p=0; p<4; ++p){
            int idx = colorIndex(face[f][p]);
            if(idx < 0) return false;
            counts[idx]++;
        }
    }
    for(int i=0;i<6;++i) if(counts[i] != 4) return false;
    return true;
}

bool Cubo::igual(const Cubo& outro) const {
    return face == outro.face;
}

void Cubo::printStickerCounts() const {
    int counts[6] = {0,0,0,0,0,0};
    for(int f=0; f<6; ++f){
        for(int p=0; p<4; ++p){
            int idx = colorIndex(face[f][p]);
            if(idx >= 0) counts[idx]++;
        }
    }
    const char* names = "WRYOBG"; // ordem usada: W R Y O B G
    cout << "Sticker counts: ";
    for(int i=0;i<6;i++){
        cout << names[i] << ":" << counts[i] << (i<5? " ":"");
    }
    cout << endl;
}


void Cubo::rota_frente(){
    array<array<char,4>,6> old_faces = face;

    char aux = face[0][0];
    face[0][0] = face[0][2];
    face[0][2] = face[0][3];
    face[0][3] = face[0][1];
    face[0][1] = aux;

    char temp1 = face[4][1];
    char temp2 = face[4][2];
    face[4][1] = face[3][2];
    face[4][2] = face[3][0];
    face[3][0] = face[5][2];
    face[3][2] = face[5][1];
    face[5][1] = face[1][3];
    face[5][2] = face[1][0];
    face[1][0] = temp1;
    face[1][3] = temp2;

    array<pair<int,int>,16> pos_alteradas = {{
        {0,0},{0,1},{0,2},{0,3},
        {1,0},{1,3},{3,0},{3,2},
        {4,1},{4,2},{5,1},{5,2},
    }};
    atualiza_hash();
}

void Cubo::rota_costa(){
    array<array<char,4>,6> old_faces = face;

    char aux = face[2][0];
    face[2][0] = face[2][2];
    face[2][2] = face[2][3];
    face[2][3] = face[2][1];
    face[2][1] = aux;

    char temp1 = face[4][0];
    char temp2 = face[4][3];

    face[4][0] = face[1][1];
    face[4][3] = face[1][2];

    face[1][1] = face[5][3];
    face[1][2] = face[5][0];

    face[5][0] = face[3][3];
    face[5][3] = face[3][1];

    face[3][1] = temp1;
    face[3][3] = temp2;

    array<pair<int,int>,16> pos_alteradas = {{
        {2,0},{2,1},{2,2},{2,3},
        {1,1},{1,2},{3,1},{3,3},
        {4,0},{4,3},{5,0},{5,3}
    }};
    atualiza_hash();
}

void Cubo::rota_dir(){
    array<array<char,4>,6> old_faces = face;

    char aux = face[1][0];
    face[1][0] = face[1][2];
    face[1][2] = face[1][3];
    face[1][3] = face[1][1];
    face[1][1] = aux;

    char temp1 = face[0][1];
    char temp2 = face[0][2];

    face[0][1] = face[4][1];
    face[0][2] = face[4][2];

    face[4][1] = face[2][0];
    face[4][2] = face[2][3];

    face[2][0] = face[5][1];
    face[2][3] = face[5][2];

    face[5][1] = temp1;
    face[5][2] = temp2;

    array<pair<int,int>,16> pos_alteradas = {{
        {1,0},{1,1},{1,2},{1,3},
        {0,1},{0,2},{2,0},{2,3},
        {4,1},{4,2},{5,1},{5,2}
    }};
    atualiza_hash();
}

void Cubo::rota_esq(){
    array<array<char,4>,6> old_faces = face;

    char aux = face[3][0];
    face[3][0] = face[3][2];
    face[3][2] = face[3][3];
    face[3][3] = face[3][1];
    face[3][1] = aux;

    char temp1 = face[0][0];
    char temp2 = face[0][3];

    face[0][0] = face[5][0];
    face[0][3] = face[5][3];

    face[5][0] = face[2][1];
    face[5][3] = face[2][2];

    face[2][1] = face[4][0];
    face[2][2] = face[4][3];

    face[4][0] = temp1;
    face[4][3] = temp2;

    array<pair<int,int>,16> pos_alteradas = {{
        {3,0},{3,1},{3,2},{3,3},
        {0,0},{0,3},{2,1},{2,2},
        {4,0},{4,3},{5,0},{5,3}
    }};
    atualiza_hash();
}

void Cubo::rota_topo(){
    array<array<char,4>,6> old_faces = face;

    char aux = face[4][0];
    face[4][0] = face[4][2];
    face[4][2] = face[4][3];
    face[4][3] = face[4][1];
    face[4][1] = aux;

    char temp1 = face[0][0];
    char temp2 = face[0][1];

    face[0][0] = face[1][0];
    face[0][1] = face[1][1];

    face[1][0] = face[2][0];
    face[1][1] = face[2][1];

    face[2][0] = face[3][0];
    face[2][1] = face[3][1];

    face[3][0] = temp1;
    face[3][1] = temp2;

    array<pair<int,int>,16> pos_alteradas = {{
        {4,0},{4,1},{4,2},{4,3},
        {0,0},{0,1},{1,0},{1,1},
        {2,0},{2,1},{3,0},{3,1}
    }};
    atualiza_hash();
}

void Cubo::rota_base(){
    array<array<char,4>,6> old_faces = face;

    char aux = face[5][0];
    face[5][0] = face[5][2];
    face[5][2] = face[5][3];
    face[5][3] = face[5][1];
    face[5][1] = aux;

    char temp1 = face[0][2];
    char temp2 = face[0][3];

    face[0][2] = face[3][2];
    face[0][3] = face[3][3];

    face[3][2] = face[2][2];
    face[3][3] = face[2][3];

    face[2][2] = face[1][2];
    face[2][3] = face[1][3];

    face[1][2] = temp1;
    face[1][3] = temp2;

    array<pair<int,int>,16> pos_alteradas = {{
        {5,0},{5,1},{5,2},{5,3},
        {0,2},{0,3},{1,2},{1,3},
        {2,2},{2,3},{3,2},{3,3}
    }};
    atualiza_hash();
}


void Cubo::embaralha(){
    rota_frente();
    rota_dir();
    rota_costa();
}  
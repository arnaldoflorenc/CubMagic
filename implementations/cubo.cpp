#include <iostream>
#include <algorithm>
#include "../class/Cubo.h"
using namespace std;

Cubo::Cubo(){
    //Estado resolvido: cantos em ordem, orientação 0
    for (int i = 0; i < 8; i++) {
        posicoes[i] = i;
        orientacoes[i] = 0;
    }
}

size_t Cubo::hash() const{//transforma o cubo em um número único
    size_t h = 0;
    for (int i = 0; i < 8; i++) {
        h = h * 31 + posicoes[i]; //multiplica por um primo
        h = h * 31 + orientacoes[i];
    }
    return h;
}

void Cubo::printar() const { //print para debug
    cout << "Posições: ";
    for (int i = 0; i < 8; i++) {
        cout << posicoes[i] << " ";
    }
    cout << endl;
    
    cout << "Orientações: ";
    for (int i = 0; i < 8; i++) {
        cout << orientacoes[i] << " ";
    }
    cout << endl;
}

bool Cubo::resolvido() const{//função que verifica se o cubo está resolvido
    for (int i = 0; i < 8; i++) {
        if (posicoes[i] != i || orientacoes[i] != 0) {
            return false;
        }
    }
    return true;
}

void Cubo::rota_frente() {
    // F (sentido horário)
    array<int, 4> corners = {0, 1, 5, 4}; // URF, ULF, DLF, DRF
    
    int temp_pos = posicoes[corners[0]];
    posicoes[corners[0]] = posicoes[corners[3]];
    posicoes[corners[3]] = posicoes[corners[2]];
    posicoes[corners[2]] = posicoes[corners[1]];
    posicoes[corners[1]] = temp_pos;
}

void Cubo::rota_costa() {
    // B (sentido horário)
    array<int, 4> corners = {3, 2, 6, 7}; // URB, ULB, DLB, DRB
    
    int temp_pos = posicoes[corners[0]];
    posicoes[corners[0]] = posicoes[corners[3]];
    posicoes[corners[3]] = posicoes[corners[2]];
    posicoes[corners[2]] = posicoes[corners[1]];
    posicoes[corners[1]] = temp_pos;
}

void Cubo::rota_frente_anti() {
    //inverso de F
    array<int, 4> corners = {0, 1, 5, 4}; // URF, ULF, DLF, DRF
    
    int temp_pos = posicoes[corners[0]];
    posicoes[corners[0]] = posicoes[corners[1]];
    posicoes[corners[1]] = posicoes[corners[2]];
    posicoes[corners[2]] = posicoes[corners[3]];
    posicoes[corners[3]] = temp_pos;
}

void Cubo::rota_costa_anti() {
    //inverso de B
    array<int, 4> corners = {3, 2, 6, 7}; // URB, ULB, DLB, DRB
    
    int temp_pos = posicoes[corners[0]];
    posicoes[corners[0]] = posicoes[corners[1]];
    posicoes[corners[1]] = posicoes[corners[2]];
    posicoes[corners[2]] = posicoes[corners[3]];
    posicoes[corners[3]] = temp_pos;
}

void Cubo::rota_dir() {
    // R (sentido horário)
    array<int, 4> corners = {0, 3, 7, 4}; // URF, URB, DRB, DRF
    
    int temp_pos = posicoes[corners[0]];
    posicoes[corners[0]] = posicoes[corners[3]];
    posicoes[corners[3]] = posicoes[corners[2]];
    posicoes[corners[2]] = posicoes[corners[1]];
    posicoes[corners[1]] = temp_pos;
}

void Cubo::rota_esq() {
    // L (sentido horário)
    array<int, 4> corners = {1, 2, 6, 5}; // ULF, ULB, DLB, DLF
    
    int temp_pos = posicoes[corners[0]];
    posicoes[corners[0]] = posicoes[corners[3]];
    posicoes[corners[3]] = posicoes[corners[2]];
    posicoes[corners[2]] = posicoes[corners[1]];
    posicoes[corners[1]] = temp_pos;
}

void Cubo::rota_dir_anti() {
    //inverso de R
    array<int, 4> corners = {0, 3, 7, 4}; // URF, URB, DRB, DRF
    
    int temp_pos = posicoes[corners[0]];
    posicoes[corners[0]] = posicoes[corners[1]];
    posicoes[corners[1]] = posicoes[corners[2]];
    posicoes[corners[2]] = posicoes[corners[3]];
    posicoes[corners[3]] = temp_pos;
}

void Cubo::rota_esq_anti() {
    //inverso de L
    array<int, 4> corners = {1, 2, 6, 5}; // ULF, ULB, DLB, DLF
    
    int temp_pos = posicoes[corners[0]];
    posicoes[corners[0]] = posicoes[corners[1]];
    posicoes[corners[1]] = posicoes[corners[2]];
    posicoes[corners[2]] = posicoes[corners[3]];
    posicoes[corners[3]] = temp_pos;
}

void Cubo::rota_topo() {
    //U (sentido horário)
    array<int, 4> corners = {0, 3, 2, 1}; // URF, URB, ULB, ULF
    
    int temp_pos = posicoes[corners[0]];
    posicoes[corners[0]] = posicoes[corners[3]];
    posicoes[corners[3]] = posicoes[corners[2]];
    posicoes[corners[2]] = posicoes[corners[1]];
    posicoes[corners[1]] = temp_pos;
}

void Cubo::rota_base() {
    //D (sentido horário)
    array<int, 4> corners = {4, 7, 6, 5}; // DRF, DRB, DLB, DLF
    
    int temp_pos = posicoes[corners[0]];
    posicoes[corners[0]] = posicoes[corners[3]];
    posicoes[corners[3]] = posicoes[corners[2]];
    posicoes[corners[2]] = posicoes[corners[1]];
    posicoes[corners[1]] = temp_pos;
}

void Cubo::rota_topo_anti() {
    //inverso de U
    array<int, 4> corners = {0, 3, 2, 1}; // URF, URB, ULB, ULF
    
    int temp_pos = posicoes[corners[0]];
    posicoes[corners[0]] = posicoes[corners[1]];
    posicoes[corners[1]] = posicoes[corners[2]];
    posicoes[corners[2]] = posicoes[corners[3]];
    posicoes[corners[3]] = temp_pos;
}

void Cubo::rota_base_anti() {
    //inverso de D
    array<int, 4> corners = {4, 7, 6, 5}; // DRF, DRB, DLB, DLF
    
    int temp_pos = posicoes[corners[0]];
    posicoes[corners[0]] = posicoes[corners[1]];
    posicoes[corners[1]] = posicoes[corners[2]];
    posicoes[corners[2]] = posicoes[corners[3]];
    posicoes[corners[3]] = temp_pos;
}

bool Cubo::isCornerCorrect(int corner) const {
    return posicoes[corner] == corner;
}

bool Cubo::isCornerOriented(int corner) const {
    return orientacoes[corner] == 0;
}

int Cubo::countCorrectCorners() const {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        if (isCornerCorrect(i)) {
            count++;
        }
    }
    return count;
}

int Cubo::countOrientedCorners() const {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        if (isCornerOriented(i)) {
            count++;
        }
    }
    return count;
}

int Cubo::countSolvedFaces() const {
    int solved = 0;
    
    int correctCount = countCorrectCorners();
    if (correctCount == 8) return 6; // Cubo totalmente resolvido
    
    if (correctCount >= 6) return 2;
    if (correctCount >= 4) return 1;
    
    return 0;
}

void Cubo::embaralha(){//emarralha o cubo com uma sequência fixa de movimentos
    rota_dir_anti();
    rota_topo_anti();
    rota_dir_anti();
    rota_frente_anti();
    rota_dir_anti();
    rota_frente_anti();
    rota_topo_anti();
    rota_dir_anti();
    rota_dir_anti();
    rota_topo_anti();
    rota_dir_anti();
    rota_topo_anti();
    rota_dir_anti();
    rota_topo_anti();
    rota_dir_anti();
    rota_topo_anti();
    rota_topo_anti();
}

array<array<char, 4>, 6> Cubo::para_faces() const { //função de teste
    array<array<char, 4>, 6> faces;
    
    char cores_faces[6] = {'W', 'R', 'Y', 'O', 'B', 'G'};

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {

            faces[i][j] = cores_faces[i]; // Mostra face
        }
    }
    
    return faces;
}

void Cubo::printar_bonito() const {//função de teste
    auto faces = para_faces();
    string nomes[6] = {"FRENTE", "DIREITA", "COSTA", "ESQUERDA", "TOPO", "BASE"};
    
    for(int i = 0; i < 6; i++) {
        cout << nomes[i] << ": ";
        for(int j = 0; j < 4; j++) {
            cout << faces[i][j];
        }
        cout << endl;
    }
    cout << endl;
}
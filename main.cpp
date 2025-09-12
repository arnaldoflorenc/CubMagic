#include <iostream>
#include "class/Cubo.h"
#include "class/Solver.h"
using namespace std;

int main(){
    Cubo cub;

    // face 0= branco face 1= vermelho face 2=amarelo face 3=laranja face 4=azul face 5=verde
    cout<<"Cubo (estado inicial):\n";
    cub.printar();

    cub.embaralha();
    cout<<endl;

    cub.printar();

    vector<string> solucao = cubo_resolve(cub);
    if (!solucao.empty()){
        cout<<"\nSolução (movimentos): ";
        for (auto &m : solucao){
            cout<<m<<" ";
        } 
        cout<<"\n";
    } else {
        cout<<"Não encontrou solução"<<endl;
    }

    for(const auto& monta_aux : solucao){
        if(monta_aux == "F"){
            cub.rota_frente();
        }else if(monta_aux == "C"){
            cub.rota_costa();
        }else if(monta_aux == "D"){
            cub.rota_dir();
        }else if(monta_aux == "E"){
            cub.rota_esq();
        }else if(monta_aux == "X"){
            cub.rota_cub_dir();
        }else{
            cub.rota_cub_cima();
        }
    }
    cub.printar();

    return 0;
}
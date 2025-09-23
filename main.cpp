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

    vector<int> solucao = Aestrela(cub,  1000000, 4);
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
        if(monta_aux == 0){
            cub.rota_frente();
        }else if(monta_aux == 1){
            cub.rota_costa();
        }else if(monta_aux == 2){
            cub.rota_dir();
        }else if(monta_aux == 3){
            cub.rota_esq();
        }else if(monta_aux == 4){
            cub.rota_topo();
        }else{
            cub.rota_base();
        }
    }
    cub.printar();

    return 0;
}
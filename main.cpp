#include <iostream>
#include "class/Cubo.h"
#include "class/Solver.h"
using namespace std;

int main(){
    Cubo cub;

    // face 0= branco face 1= vermelho face 2=amarelo face 3=laranja face 4=azul face 5=verde
    cout<<"Cubo (estado inicial):\n";

    cub.embaralha();
    cub.printar_bonito();
    cout<<endl;

    vector<int> solucao = Aestrela(cub);
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
            cub.rota_frente_anti();
        }else if(monta_aux == 2){
            cub.rota_dir();
        }else if(monta_aux == 3){
            cub.rota_dir_anti();
        }else if(monta_aux == 4){
            cub.rota_topo();
        }else if(monta_aux == 5){
            cub.rota_topo_anti();
        }else if(monta_aux == 6){
            cub.rota_base();
        }else if(monta_aux == 7){
            cub.rota_base_anti();
        }else if(monta_aux == 8){
            cub.rota_esq();
        }else if(monta_aux == 9){
            cub.rota_esq_anti();
        }else if(monta_aux == 10){
            cub.rota_costa();
        }else{
            cub.rota_costa_anti();
        }
    }
    cub.printar_bonito();

    return 0;
}
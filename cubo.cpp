#include <iostream>
#include <array>
#include <vector>
#include <functional>
#include <stack>
#include <string>
#include <unordered_set>
using namespace std;



class Cubo{
    private: 
        array<array<char,4>,6> face;
        int custo_mov;

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
            cout<<endl;
        }

        bool resolvido(){
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

        string transfString(){
            string transform;
            transform.reserve(24);
            for(int i = 0; i < 6; i++){
                for(int j = 0; j < 4; j++){
                    transform.push_back(face[i][j]);
                }
            }
            return transform;
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

        void rota_dir(){
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

        void rota_esq(){
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

        void rota_cub_dir(){
            rota_costa();
            rota_frente();
        }

        void rota_cub_cima(){
            rota_dir();
            rota_esq();
        }

        void embaralha(){
            rota_costa();
            rota_dir();
        }
        
};

typedef struct Estado{
    Cubo cube;
    int profund;
    vector<string> caminho;
}E;

vector<pair<string, function<void(Cubo&)>>> get_moves(){
    return{
        {"F", [](Cubo& c){c.rota_frente();}},
        {"C", [](Cubo& c){c.rota_costa();}},
        {"D", [](Cubo& c){c.rota_dir();}},
        {"E", [](Cubo& c){c.rota_esq();}},
        {"Y", [](Cubo& c){c.rota_cub_cima();}},
        {"X", [](Cubo& c){c.rota_cub_dir();}}
    };
}

vector<string> cubo_resolve(Cubo inicio){ //busca uma solução enquanto n tiver chego no limite estabelecido
    auto movimentos = get_moves();
    unordered_set<string> visitados;

    stack<E> pilha;
    pilha.push({inicio, 0, {}});

    while(!pilha.empty()){
        E estado_atual =  pilha.top();
        pilha.pop();
        

        if(estado_atual.cube.resolvido()){
            return estado_atual.caminho;
        }

        if(estado_atual.profund == 14){
            continue;
        }
        
        string estado = estado_atual.cube.transfString();
        if(visitados.count(estado)){
            continue;
        }     
        visitados.insert(estado);

        for(auto &mov : movimentos){
            Cubo prox = estado_atual.cube;
            mov.second(prox);

            vector<string> n_caminho = estado_atual.caminho;
            n_caminho.push_back(mov.first);

            pilha.push(E{prox, estado_atual.profund + 1, n_caminho});
        }
    }    
    return {};
}


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

    for(auto monta_aux : solucao){
        if(monta_aux == "F"){
            cub.rota_frente();
            cub.printar();
        }else if(monta_aux == "C"){
            cub.rota_costa();
            cub.printar();
        }else if(monta_aux == "D"){
            cub.rota_dir();
            cub.printar();
        }else if(monta_aux == "E"){
            cub.rota_esq();
            cub.printar();
        }else if(monta_aux == "X"){
            cub.rota_cub_dir();
            cub.printar();
        }else{
            cub.rota_cub_cima();
            cub.printar();
        }
    }
    cub.printar();

    return 0;
}
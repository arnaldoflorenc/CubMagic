
#include "../class/Solver.h"
#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <stack>
#include <memory>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include "../class/bloom.h"

using namespace std;

vector<M> get_moves(){
    return{
        {0, [](Cubo& c){c.rota_frente(); }, 1},
        {1, [](Cubo& c){c.rota_costa(); },1},
        {2, [](Cubo& c){c.rota_dir(); },1},
        {3, [](Cubo& c){c.rota_esq(); },1},
        {4, [](Cubo& c){c.rota_topo(); },1},
        {5, [](Cubo& c){c.rota_base(); },1}
    };
}

int inverso(int mov) {
    return mov ^ 1;
}

// Função para reconstruir o caminho como vetor de int
vector<int> reconstruir_caminho(shared_ptr<N> no_final) {
    vector<int> caminho;
    while (no_final && no_final->mov != -1) {
        caminho.push_back(no_final->mov);
        no_final = no_final->anterior;
    }
    reverse(caminho.begin(), caminho.end());
    return caminho;
}

vector<int> DFS(Cubo inicio, size_t bloomSize = 4000000, int bloomHashes = 5){ //busca uma solução enquanto n tiver chego no limite estabelecido
    auto movimentos = get_moves();
    unordered_set<size_t> visitados;
    stack<shared_ptr<N>> pilha;
    pilha.push(make_shared<N>(N{inicio, 0, -1, nullptr}));
    while(!pilha.empty()){
        auto no_atual = pilha.top();
        pilha.pop();
        if(no_atual->cube.resolvido()){
            return reconstruir_caminho(no_atual);
        }
        if(no_atual->profund >= 14){
            continue;
        }
        size_t estado_hash = no_atual->cube.hash();
        if(visitados.count(estado_hash)) {
            continue;
        }
        visitados.insert(estado_hash);
        for(auto &mov : movimentos){
            // Evita movimentos inversos
            if (no_atual->mov != -1 && mov.mov == inverso(no_atual->mov)) continue;
            // Evita ciclos curtos: exemplo, não faz A-B-A
            if (no_atual->anterior && no_atual->anterior->mov == mov.mov) continue;
            // Reutiliza Cubo por referência para evitar cópia desnecessária
            shared_ptr<Cubo> prox_cubo = make_shared<Cubo>(no_atual->cube);
            mov.acao(*prox_cubo);
            pilha.push(make_shared<N>(N{*prox_cubo, no_atual->profund + 1, mov.mov, no_atual}));
        }
    }
    return {};
}

vector<int> BFS(Cubo inicio, size_t bloomSize = 4000000, int bloomHashes = 5){
    auto movimentos = get_moves();
    bloom bloomFilter(bloomSize, bloomHashes);
    unordered_set<size_t> exactSet;
    size_t exactSetInsertions = 0;
    const size_t exactSetMaxSize = 100000; // Limpa a cada 100.000 inserções
    queue<shared_ptr<N>> gila;
    gila.push(make_shared<N>(N{inicio, 0, -1, nullptr}));
    while(!gila.empty()){
        auto no_atual = gila.front();
        gila.pop();
        if(no_atual->cube.resolvido()){
            return reconstruir_caminho(no_atual);
        }
        if(no_atual->profund >= 14){
            continue;
        }
        size_t estado_hash = no_atual->cube.hash();
        if(bloomFilter.mightContain(estado_hash)) {
            if(exactSet.count(estado_hash)) {
                continue;
            }
        } else {
            bloomFilter.insert(estado_hash);
            exactSet.insert(estado_hash);
            exactSetInsertions++;
            if (exactSetInsertions >= exactSetMaxSize) {
                exactSet.clear();
                exactSetInsertions = 0;
            }
        }
        for(auto &mov : movimentos){
            if (no_atual->mov != -1 && mov.mov == inverso(no_atual->mov)) continue;
            Cubo prox = no_atual->cube;
            mov.acao(prox);
            gila.push(make_shared<N>(N{prox, no_atual->profund + mov.custo, mov.mov, no_atual}));
        }
    }
    return {};
}

class Heuristica2x2 {
private:
    unordered_map<uint32_t, int> patternDB;
    const size_t MAX_DB_SIZE = 50000;
    
    // Converte cor para índice (0-5)
    int colorToIndex(char color) const {
        switch(color) {
            case 'W': return 0; case 'R': return 1;
            case 'Y': return 2; case 'O': return 3;
            case 'B': return 4; case 'G': return 5;
            default: return 0;
        }
    }
    
    // Gera chave única baseada nos CANTOS do 2x2
    uint32_t generateCornerPattern(const Cubo& cube) const {
        uint32_t pattern = 0;
        
        // POSIÇÕES DOS CANTOS no cubo 2x2
        // Cada canto é definido por 3 posições (face, índice)
        struct Corner { int face1, pos1, face2, pos2, face3, pos3; };
        
        Corner corners[8] = {
            // Cantos superiores
            {0,0, 4,1, 3,0}, {0,2, 4,2, 1,0},
            {2,0, 4,0, 1,1}, {2,2, 4,3, 3,1},
            // Cantos inferiores  
            {0,1, 5,0, 3,2}, {0,3, 5,1, 1,2},
            {2,1, 5,3, 1,3}, {2,3, 5,2, 3,3}
        };
        
        for (int i = 0; i < 8; i++) {
            const Corner& c = corners[i];
            
            // Pega as 3 cores deste canto
            char color1 = cube.getFaceColor(c.face1, c.pos1);
            char color2 = cube.getFaceColor(c.face2, c.pos2); 
            char color3 = cube.getFaceColor(c.face3, c.pos3);
            
            // Codifica as 3 cores (3 bits cada)
            uint32_t cornerCode = (colorToIndex(color1) << 6) | 
                                 (colorToIndex(color2) << 3) | 
                                 (colorToIndex(color3));
            
            // Combina no pattern final
            pattern ^= (cornerCode << (i * 3));
        }
        
        return pattern;
    }
    
    // Heurística inteligente baseada em cantos
    int calculateCornerHeuristic(const Cubo& cube) const {
        int heuristic = 0;
        
        // 1. CANTOS NA POSIÇÃO CORRETA
        int correctCorners = countCorrectCorners(cube);
        heuristic += (8 - correctCorners) * 2;
        
        // 2. CANTOS COM ORIENTAÇÃO CORRETA  
        int orientedCorners = countOrientedCorners(cube);
        heuristic += (8 - orientedCorners) * 1;
        
        // 3. FACES RESOLVIDAS (bônus negativo)
        int solvedFaces = countSolvedFaces(cube);
        heuristic -= solvedFaces * 2;
        
        return max(1, heuristic);
    }
    
    int countCorrectCorners(const Cubo& cube) const {
        int count = 0;
        // Um canto está na posição correta se suas 3 cores 
        // pertencem às 3 faces que se encontram naquela posição
        // (implementação simplificada)
        
        for (int face = 0; face < 6; face++) {
            char expected = cube.getExpectedColor(face);
            // Verifica se os cantos desta face têm cores esperadas
            if (cube.getFaceColor(face, 0) == expected) count++;
            if (cube.getFaceColor(face, 2) == expected) count++;
        }
        
        return count / 3; // Aproximação
    }
    
    int countOrientedCorners(const Cubo& cube) const {
        // Um canto está orientado corretamente se a cor "principal"
        // está na face correta
        int count = 0;
        
        for (int face = 0; face < 6; face++) {
            char expected = cube.getExpectedColor(face);
            // Verifica posições de canto
            if (cube.getFaceColor(face, 0) == expected) count++;
            if (cube.getFaceColor(face, 2) == expected) count++;
        }
        
        return count;
    }
    
    int countSolvedFaces(const Cubo& cube) const {
        int solved = 0;
        for (int face = 0; face < 6; face++) {
            char first = cube.getFaceColor(face, 0);
            bool faceSolved = true;
            for (int pos = 1; pos < 4; pos++) {
                if (cube.getFaceColor(face, pos) != first) {
                    faceSolved = false;
                    break;
                }
            }
            if (faceSolved) solved++;
        }
        return solved;
    }
    
public:
    int getHeuristic(const Cubo& cube) {
        // Gera pattern baseado nos cantos
        uint32_t patternKey = generateCornerPattern(cube);
        
        // Verifica no lookup table
        auto it = patternDB.find(patternKey);
        if (it != patternDB.end()) {
            return it->second;
        }
        
        // Calcula e armazena se não encontrou
        int h = calculateCornerHeuristic(cube);
        
        if (patternDB.size() < MAX_DB_SIZE) {
            patternDB[patternKey] = h;
        }
        
        return h;
    }
};

Heuristica2x2 he;

int heuristica(Cubo &cube) {
    return he.getHeuristic(cube);
}

vector<int> Aestrela(Cubo inicio) {
    auto movimentos = get_moves();
    
    unordered_map<size_t, int> best_g;
    best_g.reserve(500000);
    
    auto comparador = [](const shared_ptr<AestrelaNo>& a, const shared_ptr<AestrelaNo>& b) {
        return a->f > b->f;
    };
    priority_queue<shared_ptr<AestrelaNo>, vector<shared_ptr<AestrelaNo>>, decltype(comparador)> openList(comparador);
    
    // Inicialização
    int h_inicial = heuristica(inicio);
    auto no_inicial = make_shared<AestrelaNo>(inicio, 0, -1, nullptr, h_inicial, 0);
    best_g[inicio.hash()] = 0;
    openList.push(no_inicial);
    
    int nos_expandidos = 0;
    const int MAX_PROFUNDIDADE = 50;
    
    cout << "A* Iniciando..." << endl;

    
    while (!openList.empty()) {
        auto no_atual = openList.top();
        openList.pop();
        
        size_t hash_atual = no_atual->cube.hash();
        if (best_g[hash_atual] < no_atual->g) {
            continue;
        }
        
        if (no_atual->cube.resolvido()) {
            cout << "SOLUÇÃO ENCONTRADA!" << endl;
            cout << "Nós: " << nos_expandidos << ", Movimentos: " << no_atual->g << endl;
            return reconstruir_caminho(static_pointer_cast<No>(no_atual));
        }
        
        if (no_atual->g >= MAX_PROFUNDIDADE) {
            continue;
        }
        
        Cubo cubo_base = no_atual->cube;
        int mov_anterior = no_atual->mov;
        
        for (auto &mov : movimentos) {
            if (mov_anterior != -1 && mov.mov == inverso(mov_anterior)) {
                continue;
            }
            
            Cubo prox = cubo_base;
            mov.acao(prox);
            
            int g_novo = no_atual->g + 1;
            
            if (g_novo >= MAX_PROFUNDIDADE) continue;
            
            size_t prox_hash = prox.hash();
            
            auto it = best_g.find(prox_hash);
            if (it != best_g.end() && it->second <= g_novo) {
                continue;
            }
            
            best_g[prox_hash] = g_novo;
            int h_novo = heuristica(prox);
            
            auto novo_no = make_shared<AestrelaNo>(
                prox, no_atual->profund + 1, mov.mov, 
                static_pointer_cast<No>(no_atual), h_novo, g_novo
            );
            
            openList.push(novo_no);
        }
        
        
    }
    
    cout << "Não encontrou solução " << endl;
    return {};
}
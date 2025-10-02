#include <iostream>
#include "class/Cubo.h"
#include "class/Solver.h"
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/glu.h>
#include <vector>
#include <thread>
#include <chrono>
using namespace std;

// Variáveis globais para rotação do cubo inteiro
float rotX = -20.0f;
float rotY = 0.0f;
float rotZ = 0.0f;

// Converte char do cubo em cor RGB
void cuboCharToColor(char c, float rgb[3]) {
    switch(c){
        case 'W': rgb[0]=1; rgb[1]=1; rgb[2]=1; break;
        case 'Y': rgb[0]=1; rgb[1]=1; rgb[2]=0; break;
        case 'R': rgb[0]=1; rgb[1]=0; rgb[2]=0; break;
        case 'O': rgb[0]=1; rgb[1]=0.5; rgb[2]=0; break;
        case 'B': rgb[0]=0; rgb[1]=0; rgb[2]=1; break;
        case 'G': rgb[0]=0; rgb[1]=1; rgb[2]=0; break;
        default: rgb[0]=0; rgb[1]=0; rgb[2]=0; break;
    }
}

// Desenha um cubie individual
void drawCubie(float size, float x, float y, float z, float colors[6][3]) {
    float hs = size / 2.0f;
    glPushMatrix();
    glTranslatef(x, y, z);

    glBegin(GL_QUADS);
    // Frente
    glColor3fv(colors[0]);
    glVertex3f(-hs, -hs, hs); glVertex3f(hs, -hs, hs);
    glVertex3f(hs, hs, hs); glVertex3f(-hs, hs, hs);
    // Trás
    glColor3fv(colors[1]);
    glVertex3f(-hs, -hs, -hs); glVertex3f(-hs, hs, -hs);
    glVertex3f(hs, hs, -hs); glVertex3f(hs, -hs, -hs);
    // Direita
    glColor3fv(colors[2]);
    glVertex3f(hs, -hs, -hs); glVertex3f(hs, hs, -hs);
    glVertex3f(hs, hs, hs); glVertex3f(hs, -hs, hs);
    // Esquerda
    glColor3fv(colors[3]);
    glVertex3f(-hs, -hs, -hs); glVertex3f(-hs, -hs, hs);
    glVertex3f(-hs, hs, hs); glVertex3f(-hs, hs, -hs);
    // Topo
    glColor3fv(colors[4]);
    glVertex3f(-hs, hs, -hs); glVertex3f(-hs, hs, hs);
    glVertex3f(hs, hs, hs); glVertex3f(hs, hs, -hs);
    // Base
    glColor3fv(colors[5]);
    glVertex3f(-hs, -hs, -hs); glVertex3f(hs, -hs, -hs);
    glVertex3f(hs, -hs, hs); glVertex3f(-hs, -hs, hs);
    glEnd();

    glPopMatrix();
}

// Verifica se cubie pertence à face sendo animada
bool pertenceFace(int mov, int x, int y, int z) {
    switch(mov) {
        case 0: return z == 1; // frente
        case 1: return z == 0; // costas
        case 2: return x == 1; // direita
        case 3: return x == 0; // esquerda
        case 4: return y == 1; // topo
        case 5: return y == 0; // base
        default: return false;
    }
}

// Desenha cubo 2x2 com rotação de face animada
void drawCuboAnimado(const Cubo &cubo, float cubieSize, float spacing, int mov, float angle) {
    float offset = cubieSize + spacing;
    float start = -offset / 2.0f;

    for(int x=0;x<2;x++){
        for(int y=0;y<2;y++){
            for(int z=0;z<2;z++){
                float colors[6][3];
                cuboCharToColor(cubo.getFaceColor(0, y*2 + x), colors[0]); // Frente
                cuboCharToColor(cubo.getFaceColor(1, y*2 + z), colors[2]); // Direita
                cuboCharToColor(cubo.getFaceColor(2, y*2 + x), colors[1]); // Costas
                cuboCharToColor(cubo.getFaceColor(3, y*2 + z), colors[3]); // Esquerda
                cuboCharToColor(cubo.getFaceColor(4, z*2 + x), colors[4]); // Topo
                cuboCharToColor(cubo.getFaceColor(5, z*2 + x), colors[5]); // Base

                glPushMatrix();

                if(pertenceFace(mov, x, y, z)) {
                    switch(mov) {
                        case 0: glTranslatef(0,0,start+z*offset); glRotatef(angle,0,0,1); glTranslatef(0,0,-start-z*offset); break;
                        case 1: glTranslatef(0,0,start+z*offset); glRotatef(angle,0,0,1); glTranslatef(0,0,-start-z*offset); break;
                        case 2: glTranslatef(start+x*offset,0,0); glRotatef(angle,1,0,0); glTranslatef(-start-x*offset,0,0); break;
                        case 3: glTranslatef(start+x*offset,0,0); glRotatef(angle,1,0,0); glTranslatef(-start-x*offset,0,0); break;
                        case 4: glTranslatef(0,start+y*offset,0); glRotatef(angle,0,1,0); glTranslatef(0,-start-y*offset,0); break;
                        case 5: glTranslatef(0,start+y*offset,0); glRotatef(angle,0,1,0); glTranslatef(0,-start-y*offset,0); break;
                    }
                }

                glTranslatef(start + x*offset, start + y*offset, start + z*offset);
                drawCubie(cubieSize, 0,0,0, colors);

                glPopMatrix();
            }
        }
    }
}

// Anima movimento e atualiza estado
void animaMovimento(Cubo &cubo, int mov, sf::Window &window, float cubieSize, float spacing) {
    float angle = 0.0f;
    float step = 5.0f;

    while(angle < 90.0f) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        gluLookAt(4.0,4.0,6.0, 0,0,0, 0,1,0);
        glRotatef(rotX,1,0,0);
        glRotatef(rotY,0,1,0);
        glRotatef(rotZ,0,0,1);

        drawCuboAnimado(cubo, cubieSize, spacing, mov, angle);

        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        angle += step;
    }

    cubo.aplica_movimento(mov); // atualiza estado real
}

int main() {
    Cubo cubo;

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 2;
    settings.minorVersion = 1;

    sf::Window window(sf::VideoMode(800,600), "Cubo Magico 2x2", sf::Style::Default, settings);
    window.setActive(true);

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.f, 800.f/600.f, 1.f, 100.f);
    glMatrixMode(GL_MODELVIEW);

    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();
            else if(event.type == sf::Event::KeyPressed){
                // Rotação do cubo inteiro
                if(event.key.code == sf::Keyboard::Up) rotX += 2.0f;
                if(event.key.code == sf::Keyboard::Down) rotX -= 2.0f;
                if(event.key.code == sf::Keyboard::Left) rotY -= 2.0f;
                if(event.key.code == sf::Keyboard::Right) rotY += 2.0f;
                if(event.key.code == sf::Keyboard::Q) rotZ -= 2.0f;
                if(event.key.code == sf::Keyboard::E) rotZ += 2.0f;

                // Embaralhar instantâneo
                if(event.key.code == sf::Keyboard::S) {
                    cubo.embaralha(); // ou cubo.embaralha(); dependendo da função
                    cout << "Cubo embaralhado!" << endl;
                }


                // Resolver animado usando BFS real
                if(event.key.code == sf::Keyboard::R) {
                    vector<int> solucao = BFS(cubo, 1 << 20, 3); 
                    if(solucao.empty()) {
                        cout << "Cubo já está resolvido!" << endl;
                    } else {
                        for(int m : solucao) {
                            animaMovimento(cubo, m, window, 1.0f, 0.05f);
                        }
                        cout << "Cubo resolvido!" << endl;
                    }
                }

                // Resolver animado usando A*
                if(event.key.code == sf::Keyboard::T) {
                    cout << "A* iniciando..." << endl;
                    vector<int> solucao = Aestrela(cubo);
                    if(solucao.empty()) {
                        cout << "Cubo já está resolvido ou não foi possível achar solução!" << endl;
                    } else {
                        cout << "Animando solução..." << endl;
                        for(int m : solucao) {
                            animaMovimento(cubo, m, window, 1.0f, 0.05f);
                        }
                        cout << "Cubo resolvido com A*!" << endl;
                    }
                }

                // Debug
                if(event.key.code == sf::Keyboard::D) {
                    cout << "== Teste de rotações iniciado ==\n";
                    Cubo original = cubo;
                    original.printStickerCounts();
                    cout << "validar original: " << (original.validar_stickers() ? "OK" : "FALHA") << endl;

                    struct Test { string name; function<void(Cubo&)> f; function<void(Cubo&)> inv; };
                    vector<Test> tests = {
                        {"F", [](Cubo& c){ c.rota_frente(); }, [](Cubo& c){ c.rota_costa(); }},
                        {"B", [](Cubo& c){ c.rota_costa(); }, [](Cubo& c){ c.rota_frente(); }}, // if B is rota_costa
                        {"R", [](Cubo& c){ c.rota_dir(); }, [](Cubo& c){ c.rota_esq(); }},
                        {"L", [](Cubo& c){ c.rota_esq(); }, [](Cubo& c){ c.rota_dir(); }},
                        {"U", [](Cubo& c){ c.rota_topo(); }, [](Cubo& c){ c.rota_base(); }},
                        {"D", [](Cubo& c){ c.rota_base(); }, [](Cubo& c){ c.rota_topo(); }}
                    };

                    for(auto &t : tests){
                        cout << "\nTeste rot " << t.name << ":\n";
                        Cubo c = original;
                        t.f(c);
                        cout << " - apos rot: validar_stickers = " << (c.validar_stickers() ? "OK":"FALHA") << endl;
                        c.printStickerCounts();

                        // checar rot seguida da inversa
                        t.inv(c);
                        cout << " - apos rot + inv: igual original = " << (c.igual(original) ? "OK":"FALHA") << endl;

                        // checar rot 4x = identidade
                        Cubo c2 = original;
                        for(int i=0;i<4;i++) t.f(c2);
                        cout << " - rot x4: igual original = " << (c2.igual(original) ? "OK":"FALHA") << endl;
                    }

                    cout << "== Fim dos testes ==\n";
                }
            }
        }

        // Desenha cubo
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        gluLookAt(4.0,4.0,6.0,0,0,0,0,1,0);
        glRotatef(rotX,1,0,0);
        glRotatef(rotY,0,1,0);
        glRotatef(rotZ,0,0,1);

        drawCuboAnimado(cubo,1.0f,0.05f,-1,0.0f);
        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    return 0;
}

// Interface gráfica

#include <SFML/Graphics.hpp>
#include <iostream>
#include "class/Cubo.h"
#include "class/Solver.h"

sf::Color getColor(char cor) {
    switch(cor) {
        case 'W': return sf::Color::White;
        case 'R': return sf::Color(255,0,0);
        case 'Y': return sf::Color::Yellow;
        case 'O': return sf::Color(255,140,0);
        case 'B': return sf::Color::Blue;
        case 'G': return sf::Color::Green;
        default: return sf::Color::Black;
    }
}

void drawCubo(sf::RenderWindow& window, const Cubo& cubo, int offsetX = 200, int offsetY = 150) {
    int size = 50;

    // Face 0 (centro)
    for (int i = 0; i < 4; ++i) {
        sf::RectangleShape square(sf::Vector2f(size, size));
        square.setFillColor(getColor(cubo.face[0][i]));
        square.setOutlineColor(sf::Color::Black);
        square.setOutlineThickness(2);
        square.setPosition(offsetX + (i%2)*size, offsetY + (i/2)*size);
        window.draw(square);
    }
    // Face 1 (direita)
    for (int i = 0; i < 4; ++i) {
        sf::RectangleShape square(sf::Vector2f(size, size));
        square.setFillColor(getColor(cubo.face[1][i]));
        square.setOutlineColor(sf::Color::Black);
        square.setOutlineThickness(2);
        square.setPosition(offsetX + 2*size + (i%2)*size, offsetY + (i/2)*size);
        window.draw(square);
    }
    // Face 2 (costas)
    for (int i = 0; i < 4; ++i) {
        sf::RectangleShape square(sf::Vector2f(size, size));
        square.setFillColor(getColor(cubo.face[2][i]));
        square.setOutlineColor(sf::Color::Black);
        square.setOutlineThickness(2);
        square.setPosition(offsetX + 4*size + (i%2)*size, offsetY + (i/2)*size);
        window.draw(square);
    }
    // Face 3 (esquerda)
    for (int i = 0; i < 4; ++i) {
        sf::RectangleShape square(sf::Vector2f(size, size));
        square.setFillColor(getColor(cubo.face[3][i]));
        square.setOutlineColor(sf::Color::Black);
        square.setOutlineThickness(2);
        square.setPosition(offsetX - 2*size + (i%2)*size, offsetY + (i/2)*size);
        window.draw(square);
    }
    // Face 4 (acima)
    for (int i = 0; i < 4; ++i) {
        sf::RectangleShape square(sf::Vector2f(size, size));
        square.setFillColor(getColor(cubo.face[4][i]));
        square.setOutlineColor(sf::Color::Black);
        square.setOutlineThickness(2);
        square.setPosition(offsetX + (i%2)*size, offsetY - 2*size + (i/2)*size);
        window.draw(square);
    }
    // Face 5 (abaixo)
    for (int i = 0; i < 4; ++i) {
        sf::RectangleShape square(sf::Vector2f(size, size));
        square.setFillColor(getColor(cubo.face[5][i]));
        square.setOutlineColor(sf::Color::Black);
        square.setOutlineThickness(2);
        square.setPosition(offsetX + (i%2)*size, offsetY + 2*size + (i/2)*size);
        window.draw(square);
    }
}

void aplicarMovimento(Cubo& cubo, const std::string& mov) {
    if(mov == "F") cubo.rota_frente();
    else if(mov == "C") cubo.rota_costa();
    else if(mov == "D") cubo.rota_dir();
    else if(mov == "E") cubo.rota_esq();
    else if(mov == "X") cubo.rota_cub_dir();
    else cubo.rota_cub_cima();
}

int main() {
    Cubo cubo;
    cubo.embaralha();

    // Calcula solução inicial
    std::vector<std::string> solucao = BFS(cubo);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Cubo Magico com Animacao");
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Erro carregando fonte!" << std::endl;
    }

    sf::Clock clock;
    size_t movimentoAtual = 0;
    float delay = 1.0f; // segundos entre cada movimento

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Reinicia com tecla R
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                cubo.embaralha();
                solucao = BFS(cubo);
                movimentoAtual = 0;
                clock.restart();
            }
        }

        // Avança para o próximo movimento
        if (movimentoAtual < solucao.size() && clock.getElapsedTime().asSeconds() > delay) {
            aplicarMovimento(cubo, solucao[movimentoAtual]);
            movimentoAtual++;
            clock.restart();
        }

        window.clear();

        // Desenha o cubo
        drawCubo(window, cubo);

        // Mostra a lista de movimentos
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::White);

        std::string moves = "Solucao: ";
        for (size_t i = 0; i < solucao.size(); i++) {
            if (i == movimentoAtual)
                moves += "[" + solucao[i] + "] ";
            else
                moves += solucao[i] + " ";
        }
        moves += "\nPressione R para reiniciar";
        text.setString(moves);
        text.setPosition(50, 500);
        window.draw(text);

        window.display();
    }

    return 0;
}
#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Menu {
public:
    Menu(sf::Font& font);

    void dibujar(sf::RenderWindow& window);

private:
    sf::Font& font;

    void dibujarTexto(sf::RenderWindow& window, std::string str, int x, int y, int size = 20);
};
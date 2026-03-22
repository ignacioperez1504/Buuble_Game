#include "Menu.h"

// Constructor
Menu::Menu(sf::Font& font) : font(font) {}

// Dibujar texto auxiliar
void Menu::dibujarTexto(sf::RenderWindow& window, std::string str, int x, int y, int size) {
    sf::Text text(font, str, size);
    text.setPosition(sf::Vector2f(x, y));
    text.setFillColor(sf::Color::White);
    window.draw(text);
}

// Dibujar menú completo
void Menu::dibujar(sf::RenderWindow& window) {
    dibujarTexto(window, "THE MIND", 350, 100, 40);
    dibujarTexto(window, "Selecciona numero de jugadores", 250, 200, 25);

    dibujarTexto(window, "Presiona 2 para 2 jugadores", 280, 300);
    dibujarTexto(window, "Presiona 3 para 3 jugadores", 280, 340);
    dibujarTexto(window, "Presiona 4 para 4 jugadores", 280, 380);
}
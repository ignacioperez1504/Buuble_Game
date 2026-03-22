#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Jugador.h"
#include "Carta.h"
#include "Menu.h"

class JuegoVisual {
private:
    sf::RenderWindow window;
    sf::Font font;

    Menu menu;

    int vidas;
    int nivel;
    int numJugadores;

    std::vector<Jugador> jugadores;

    bool enMenu;
    bool enOverlay;
    bool viendoCartas;

    int jugadorActual;
    int jugadorSeleccionado;
    int ultimaCarta;

    // 🔥 NUEVO
    std::string inputTexto;

public:
    JuegoVisual();
    void ejecutar();

private:
    void iniciarNivel();
    int getMinGlobal();

    // 🔥 NUEVO
    void jugarDesdeInput(std::string input);

    void manejarEventos();

    void dibujarTexto(std::string str, int x, int y, int size = 20);
    void dibujarOverlay();
    void dibujarJuego();
};
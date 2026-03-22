#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Jugador.h"
#include "Carta.h"

class JuegoVisual {
private:
    sf::RenderWindow window;
    sf::Font font;

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

public:
    JuegoVisual();

    void ejecutar();

private:
    void iniciarNivel();
    int getMinGlobal();
    void jugarTurno(int jugador);

    void manejarEventos();

    void dibujarTexto(std::string str, int x, int y, int size = 20);
    void dibujarMenu();
    void dibujarOverlay();
    void dibujarJuego();
};
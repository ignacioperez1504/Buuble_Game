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
    int comodines;

    // 🔥 NUEVO
    std::string inputTexto;
    
    // Cartas mostradas/ocultas en overlay
    std::vector<bool> cartasVisibles;
    std::vector<sf::RectangleShape> rectangulosCartas;
    std::string mensajeEstado;
    sf::Color colorMensaje;
    sf::Clock relojMensaje;
    bool mostrarMensaje;
    
    // Historial de cartas jugadas
    std::vector<int> cartasJugadas;

    // Pantalla inicio de nivel
    bool mostrandoNivel;
    sf::Clock relojNivel;

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
    void usarComodin();
    void verificarFinNivel();
};
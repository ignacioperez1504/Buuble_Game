#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <iostream>

#include "Carta.h"
#include "Jugador.h"

class JuegoVisual {
private:
    sf::RenderWindow window;
    sf::Font font;

    int vidas = 3;
    int nivel = 1;
    int numJugadores = 0;

    std::vector<Jugador> jugadores;

    bool enMenu = true;
    bool enOverlay = false;
    bool viendoCartas = false;

    int jugadorActual = 0;

    // 🔥 NUEVO
    int jugadorSeleccionado = -1;
    int ultimaCarta = 0;

public:
    JuegoVisual() :
        window(sf::VideoMode(sf::Vector2u(900, 600)), "The Mind") {

        if (!font.openFromFile("arial.ttf")) {
            std::cout << "Error cargando fuente\n";
        }
    }

    void iniciarNivel() {
        std::vector<Carta> mazo;

        for (int i = 1; i <= 100; i++) {
            mazo.push_back(Carta(i));
        }

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(mazo.begin(), mazo.end(), g);

        int index = 0;

        jugadores.clear();
        jugadores.resize(numJugadores);

        for (int i = 0; i < nivel; i++) {
            for (auto& j : jugadores) {
                j.recibirCarta(mazo[index++]);
            }
        }

        for (auto& j : jugadores) {
            j.ordenarMano();
        }

        ultimaCarta = 0;

        jugadorActual = 0;
        enOverlay = true;
    }

    int getMinGlobal() {
        int min = 101;

        for (auto& j : jugadores) {
            if (j.tieneCartas()) {
                int val = j.verCarta().getValor();
                if (val < min) min = val;
            }
        }

        return (min == 101) ? -1 : min;
    }

    void jugarTurno(int jugador) {
        if (jugador < 0 || jugador >= numJugadores) return;

        if (!jugadores[jugador].tieneCartas()) return;

        int carta = jugadores[jugador].verCarta().getValor();
        jugadores[jugador].jugarCarta();

        int minGlobal = getMinGlobal();

        if (minGlobal != -1 && carta > minGlobal) {
            vidas--;

            // eliminar cartas menores incorrectas
            for (auto& j : jugadores) {
                while (j.tieneCartas() &&
                       j.verCarta().getValor() < carta) {
                    j.jugarCarta();
                }
            }
        }

        ultimaCarta = carta;
    }

    void dibujarTexto(std::string str, int x, int y, int size = 20) {
        sf::Text text(font, str, size);
        text.setPosition(sf::Vector2f(x, y));
        text.setFillColor(sf::Color::White);
        window.draw(text);
    }

    void dibujarMenu() {
        dibujarTexto("THE MIND", 350, 100, 40);
        dibujarTexto("Selecciona numero de jugadores", 250, 200, 25);

        dibujarTexto("Presiona 2 para 2 jugadores", 280, 300);
        dibujarTexto("Presiona 3 para 3 jugadores", 280, 340);
        dibujarTexto("Presiona 4 para 4 jugadores", 280, 380);
    }

    void dibujarOverlay() {
        dibujarTexto(
            "Jugador " + std::to_string(jugadorActual) + " mira tus cartas",
            250, 100, 25
        );

        float x = 100;

        Jugador copia = jugadores[jugadorActual];
        while (copia.tieneCartas()) {
            int v = copia.jugarCarta().getValor();

            sf::RectangleShape r(sf::Vector2f(60,80));
            r.setPosition(sf::Vector2f(x, 200));
            r.setFillColor(sf::Color::White);
            window.draw(r);

            sf::Text t(font, std::to_string(v), 20);
            t.setFillColor(sf::Color::Black);
            t.setPosition(sf::Vector2f(x+15,220));
            window.draw(t);

            x += 80;
        }

        dibujarTexto("SPACE para continuar", 300, 450);
    }

    void dibujarJuego() {
        dibujarTexto("Nivel: " + std::to_string(nivel), 50, 20);
        dibujarTexto("Vidas: " + std::to_string(vidas), 200, 20);

        dibujarTexto("Presiona numero de jugador y ENTER", 50, 60);

        if (jugadorSeleccionado != -1) {
            dibujarTexto("Jugador elegido: " + std::to_string(jugadorSeleccionado), 50, 100);
        }

        dibujarTexto("Ultima carta: " + std::to_string(ultimaCarta), 50, 140);
    }

    void manejarEventos() {
        while (auto event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (event->is<sf::Event::KeyPressed>()) {
                auto key = event->getIf<sf::Event::KeyPressed>();

                // MENU
                if (enMenu) {
                    if (key->code == sf::Keyboard::Key::Num2) numJugadores = 2;
                    if (key->code == sf::Keyboard::Key::Num3) numJugadores = 3;
                    if (key->code == sf::Keyboard::Key::Num4) numJugadores = 4;

                    if (numJugadores > 0) {
                        enMenu = false;
                        iniciarNivel();
                    }
                }

                // OVERLAY
                else if (enOverlay) {
                    if (key->code == sf::Keyboard::Key::Space) {
                        jugadorActual++;
                        if (jugadorActual >= numJugadores) {
                            enOverlay = false;
                        }
                    }
                }

                // VER CARTAS
                else if (viendoCartas) {

                    if (key->code >= sf::Keyboard::Key::Num0 &&
                        key->code <= sf::Keyboard::Key::Num9) {

                        int elegido = static_cast<int>(key->code)
                                     - static_cast<int>(sf::Keyboard::Key::Num0);

                        if (elegido >= 0 && elegido < numJugadores) {
                            jugadorActual = elegido;
                            enOverlay = true;
                            viendoCartas = false;
                        }
                    }
                }

                // JUEGO
                else {

                    if (key->code == sf::Keyboard::Key::V) {
                        viendoCartas = true;
                    }

                    // seleccionar jugador
                    if (key->code >= sf::Keyboard::Key::Num0 &&
                        key->code <= sf::Keyboard::Key::Num9) {

                        int elegido = static_cast<int>(key->code)
                                     - static_cast<int>(sf::Keyboard::Key::Num0);

                        if (elegido >= 0 && elegido < numJugadores) {
                            jugadorSeleccionado = elegido;
                        }
                    }

                    // ejecutar jugada
                    if (key->code == sf::Keyboard::Key::Enter &&
                        jugadorSeleccionado != -1) {

                        jugarTurno(jugadorSeleccionado);
                        jugadorSeleccionado = -1;
                    }
                }
            }
        }
    }

    void ejecutar() {
        while (window.isOpen()) {

            manejarEventos();

            window.clear(sf::Color(30, 30, 30));

            if (enMenu) dibujarMenu();
            else if (enOverlay) dibujarOverlay();
            else if (viendoCartas) dibujarTexto("Selecciona jugador...", 300, 300);
            else dibujarJuego();

            window.display();
        }
    }
};
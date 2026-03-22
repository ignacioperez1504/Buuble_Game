#include "JuegoVisual.h"
#include <algorithm>
#include <random>
#include <iostream>

// 🔹 Constructor
JuegoVisual::JuegoVisual() :
    window(sf::VideoMode(sf::Vector2u(900, 600)), "The Mind"),
    vidas(3),
    nivel(1),
    numJugadores(0),
    enMenu(true),
    enOverlay(false),
    viendoCartas(false),
    jugadorActual(0),
    jugadorSeleccionado(-1),
    ultimaCarta(0)
{
    if (!font.openFromFile("arial.ttf")) {
        std::cout << "Error cargando fuente\n";
    }
}

// 🔹 Iniciar nivel
void JuegoVisual::iniciarNivel() {
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

// 🔹 Obtener mínimo global
int JuegoVisual::getMinGlobal() {
    int min = 101;

    for (auto& j : jugadores) {
        if (j.tieneCartas()) {
            int val = j.verCarta().getValor();
            if (val < min) min = val;
        }
    }

    return (min == 101) ? -1 : min;
}

// 🔹 Jugar turno
void JuegoVisual::jugarTurno(int jugador) {
    if (jugador < 0 || jugador >= numJugadores) return;

    if (!jugadores[jugador].tieneCartas()) return;

    int carta = jugadores[jugador].verCarta().getValor();
    jugadores[jugador].jugarCarta();

    int minGlobal = getMinGlobal();

    if (minGlobal != -1 && carta > minGlobal) {
        vidas--;

        for (auto& j : jugadores) {
            while (j.tieneCartas() &&
                   j.verCarta().getValor() < carta) {
                j.jugarCarta();
            }
        }
    }

    ultimaCarta = carta;
}

// 🔹 Dibujar texto
void JuegoVisual::dibujarTexto(std::string str, int x, int y, int size) {
    sf::Text text(font, str, size);
    text.setPosition(sf::Vector2f(x, y));
    text.setFillColor(sf::Color::White);
    window.draw(text);
}

// 🔹 Dibujar menú
void JuegoVisual::dibujarMenu() {
    dibujarTexto("THE MIND", 350, 100, 40);
    dibujarTexto("Selecciona numero de jugadores", 250, 200, 25);

    dibujarTexto("Presiona 2 para 2 jugadores", 280, 300);
    dibujarTexto("Presiona 3 para 3 jugadores", 280, 340);
    dibujarTexto("Presiona 4 para 4 jugadores", 280, 380);
}

// 🔹 Overlay (mostrar cartas)
void JuegoVisual::dibujarOverlay() {
    dibujarTexto(
        "Jugador " + std::to_string(jugadorActual) + " mira tus cartas",
        250, 100, 25
    );

    float x = 100;

    Jugador copia = jugadores[jugadorActual];
    while (copia.tieneCartas()) {
        int v = copia.jugarCarta().getValor();

        sf::RectangleShape r(sf::Vector2f(60, 80));
        r.setPosition(sf::Vector2f(x, 200));
        r.setFillColor(sf::Color::White);
        window.draw(r);

        sf::Text t(font, std::to_string(v), 20);
        t.setFillColor(sf::Color::Black);
        t.setPosition(sf::Vector2f(x + 15, 220));
        window.draw(t);

        x += 80;
    }

    dibujarTexto("SPACE para continuar", 300, 450);
}

// 🔹 Dibujar juego
void JuegoVisual::dibujarJuego() {
    dibujarTexto("Nivel: " + std::to_string(nivel), 50, 20);
    dibujarTexto("Vidas: " + std::to_string(vidas), 200, 20);

    dibujarTexto("Presiona numero de jugador y ENTER", 50, 60);

    if (jugadorSeleccionado != -1) {
        dibujarTexto("Jugador elegido: " + std::to_string(jugadorSeleccionado), 50, 100);
    }

    dibujarTexto("Ultima carta: " + std::to_string(ultimaCarta), 50, 140);
}

// 🔹 Manejo de eventos
void JuegoVisual::manejarEventos() {
    while (auto event = window.pollEvent()) {

        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (event->is<sf::Event::KeyPressed>()) {
            auto key = event->getIf<sf::Event::KeyPressed>();

            if (!key) continue;

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

                if (key->code >= sf::Keyboard::Key::Num0 &&
                    key->code <= sf::Keyboard::Key::Num9) {

                    int elegido = static_cast<int>(key->code)
                                 - static_cast<int>(sf::Keyboard::Key::Num0);

                    if (elegido >= 0 && elegido < numJugadores) {
                        jugadorSeleccionado = elegido;
                    }
                }

                if (key->code == sf::Keyboard::Key::Enter &&
                    jugadorSeleccionado != -1) {

                    jugarTurno(jugadorSeleccionado);
                    jugadorSeleccionado = -1;
                }
            }
        }
    }
}

// 🔹 Loop principal
void JuegoVisual::ejecutar() {
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
#include "JuegoVisual.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <sstream>

// Constructor
JuegoVisual::JuegoVisual() :
    window(sf::VideoMode(sf::Vector2u(900, 600)), "The Mind"),
    font(),
    menu(font),
    vidas(3),
    nivel(1),
    numJugadores(0),
    enMenu(true),
    enOverlay(false),
    viendoCartas(false),
    jugadorActual(0),
    jugadorSeleccionado(-1),
    ultimaCarta(0),
    inputTexto(""),
    cartasVisibles(),
    rectangulosCartas()
{
    if (!font.openFromFile("arial.ttf")) {
        std::cout << "Error cargando fuente\n";
    }
}

// Iniciar nivel
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

// Min global
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

// Jugar desde input tipo "23"
void JuegoVisual::jugarDesdeInput(std::string input) {
    std::stringstream ss(input);
    int carta;

    ss >> carta;
    if (ss.fail()) return;

    // Buscar jugador que tiene esa carta arriba
    int jugador = -1;

    for (int i = 0; i < numJugadores; i++) {
        if (jugadores[i].tieneCartas() &&
            jugadores[i].verCarta().getValor() == carta) {
            jugador = i;
            break;
        }
    }

    // Nadie tiene esa carta
    if (jugador == -1) return;

    int minGlobal = getMinGlobal();

    // ❌ Carta incorrecta
    if (carta != minGlobal) {
        vidas--;
        std::cout << "❌ Carta incorrecta. Vidas restantes: "
                  << vidas << "\n";

        // eliminar cartas menores
        for (auto& j : jugadores) {
            while (j.tieneCartas() &&
                   j.verCarta().getValor() < carta) {
                j.jugarCarta();
            }
        }

        if (vidas <= 0) {
            std::cout << "💀 GAME OVER\n";
            window.close();
        }

        return;
    }

    // ✅ Carta correcta
    jugadores[jugador].jugarCarta();
    std::cout << "✅ Carta correcta: " << carta << "\n";

    ultimaCarta = carta;

    // Revisar fin de nivel
    bool nivelTerminado = true;
    for (auto& j : jugadores) {
        if (j.tieneCartas()) {
            nivelTerminado = false;
            break;
        }
    }

    if (nivelTerminado) {
        nivel++;
        std::cout << "🎉 Nivel " << nivel << "\n";
        iniciarNivel();
    }
}

// Dibujar texto
void JuegoVisual::dibujarTexto(std::string str, int x, int y, int size) {
    sf::Text text(font, str, size);
    text.setPosition(sf::Vector2f(x, y));
    text.setFillColor(sf::Color::White);
    window.draw(text);
}

// Overlay
void JuegoVisual::dibujarOverlay() {
    dibujarTexto(
        "Jugador " + std::to_string(jugadorActual) + " mira tus cartas",
        250, 100, 25
    );

    float x = 100;
    int indice = 0;

    Jugador copia = jugadores[jugadorActual];
    
    // Inicializar cartas visibles si es primera vez
    if (cartasVisibles.empty()) {
        while (copia.tieneCartas()) {
            cartasVisibles.push_back(false);  // por defecto mostrar
            copia.jugarCarta();
        }
    }
    
    // Reinicializar vector de rectángulos
    rectangulosCartas.clear();
    
    // Redibujar la copia para iterar
    copia = jugadores[jugadorActual];
    indice = 0;
    
    while (copia.tieneCartas()) {
        int v = copia.jugarCarta().getValor();

        sf::RectangleShape r(sf::Vector2f(60, 80));
        r.setPosition(sf::Vector2f(x, 200));
        
        // Guardar rectángulo para detectar clics
        rectangulosCartas.push_back(r);
        
        // Mostrar u ocultar carta
        if (cartasVisibles[indice]) {
            // Carta visible
            r.setFillColor(sf::Color::White);
            window.draw(r);
            
            sf::Text t(font, std::to_string(v), 20);
            t.setFillColor(sf::Color::Black);
            t.setPosition(sf::Vector2f(x + 15, 220));
            window.draw(t);
        } else {
            // Carta oculta (color oscuro/gris)
            r.setFillColor(sf::Color(100, 100, 100));
            window.draw(r);
            
            sf::Text t(font, "?", 30);
            t.setFillColor(sf::Color::White);
            t.setPosition(sf::Vector2f(x + 20, 215));
            window.draw(t);
        }

        x += 80;
        indice++;
    }

    dibujarTexto("SPACE para continuar | CLIC para mostrar/ocultar", 200, 450);
}


// Juego
void JuegoVisual::dibujarJuego() {
    dibujarTexto("Nivel: " + std::to_string(nivel), 50, 20);
    dibujarTexto("Vidas: " + std::to_string(vidas), 200, 20);

    dibujarTexto("Escribe una carta (ej: 23)", 50, 60);
    dibujarTexto(inputTexto, 50, 100);

    dibujarTexto("Ultima carta: " + std::to_string(ultimaCarta), 50, 140);
    dibujarTexto("Presiona V para ver cartas", 50, 180);
}

// Eventos
void JuegoVisual::manejarEventos() {
    while (auto event = window.pollEvent()) {

        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        // CLIC DEL MOUSE
        if (event->is<sf::Event::MouseButtonPressed>()) {
            auto mouse = event->getIf<sf::Event::MouseButtonPressed>();
            if (mouse && mouse->button == sf::Mouse::Button::Left && enOverlay) {
                sf::Vector2f posicion(mouse->position);
                
                // Verificar qué carta fue clickeada
                for (int i = 0; i < rectangulosCartas.size(); i++) {
                    if (rectangulosCartas[i].getGlobalBounds().contains(posicion)) {
                        // Toggle mostrar/ocultar carta
                        if (i < cartasVisibles.size()) {
                            cartasVisibles[i] = !cartasVisibles[i];
                        }
                        break;
                    }
                }
            }
        }

        // INPUT TEXTO
        if (event->is<sf::Event::TextEntered>()) {
            auto text = event->getIf<sf::Event::TextEntered>();

            if (text->unicode == 8) { // backspace
                if (!inputTexto.empty())
                    inputTexto.pop_back();
            }
            else if (text->unicode < 128) {
                inputTexto += static_cast<char>(text->unicode);
            }
        }

        if (event->is<sf::Event::KeyPressed>()) {
            auto key = event->getIf<sf::Event::KeyPressed>();
            if (!key) continue;

            if (enMenu) {
                if (key->code == sf::Keyboard::Key::Num2) numJugadores = 2;
                if (key->code == sf::Keyboard::Key::Num3) numJugadores = 3;
                if (key->code == sf::Keyboard::Key::Num4) numJugadores = 4;

                if (numJugadores > 0) {
                    enMenu = false;
                    iniciarNivel();
                }
            }

            else if (enOverlay) {
                if (key->code == sf::Keyboard::Key::Space) {
                    jugadorActual++;
                    cartasVisibles.clear();  // Reset cartas para siguiente jugador
                    if (jugadorActual >= numJugadores) {
                        enOverlay = false;
                    }
                }
            }

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

            else {
                if (key->code == sf::Keyboard::Key::V) {
                    viendoCartas = true;
                }

                if (key->code == sf::Keyboard::Key::Enter) {
                    jugarDesdeInput(inputTexto);
                    inputTexto = "";
                }
            }
        }
    }
}

// Loop
void JuegoVisual::ejecutar() {
    while (window.isOpen()) {

        manejarEventos();

        window.clear(sf::Color(30, 30, 30));

        if (enMenu) menu.dibujar(window);
        else if (enOverlay) dibujarOverlay();
        else if (viendoCartas) dibujarTexto("Selecciona jugador...", 300, 300);
        else dibujarJuego();

        window.display();
    }
}
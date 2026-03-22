#ifndef JUGADOR_H
#define JUGADOR_H

#include <vector>
#include "Carta.h"

class Jugador {
private:
    std::vector<Carta> mano;

public:
    void recibirCarta(Carta c);
    void mostrarMano();
    void ordenarMano();
    Carta jugarCarta();

    Carta verCarta();
    bool tieneCartas();

    void eliminarCartaMasBaja();
};

#endif
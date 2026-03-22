#include <iostream>
#include <algorithm>
#include "Jugador.h"

void Jugador::recibirCarta(Carta c) {
    mano.push_back(c);
}

void Jugador::mostrarMano() {
    for (Carta c : mano) {
        std::cout << c.getValor() << " ";
    }
    std::cout << std::endl;
}

void Jugador::ordenarMano() {
    std::sort(mano.begin(), mano.end(), [](Carta a, Carta b) {
        return a.getValor() < b.getValor();
    });
}

Carta Jugador::jugarCarta() {
    Carta c = mano.front();
    mano.erase(mano.begin());
    return c;
}

Carta Jugador::verCarta() {
    return mano.front();
}

bool Jugador::tieneCartas() {
    return !mano.empty();
}

void Jugador::eliminarCartaMasBaja() {
    if (!mano.empty()) {
        std::cout << "(Se elimina " << mano.front().getValor() << ") ";
        mano.erase(mano.begin());
    }
}
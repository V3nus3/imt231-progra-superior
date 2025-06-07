#include "Mesa.h"

Mesa::Mesa(int num, int cap) : numeroMesa(num), capacidad(cap), ocupada(false) {}

bool Mesa::estaDisponible() const {
    return !ocupada;
}

void Mesa::ocupar() {
    ocupada = true;
}

void Mesa::liberar() {
    ocupada = false;
}
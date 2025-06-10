#include "Mesa.h"

Mesa::Mesa(int num, int cap) : numeroMesa(num), capacidad(cap), ocupada(false) {}

//Comprueba si la mesa esta disponible o no
bool Mesa::estaDisponible() const {
    return !ocupada;
}
// Ocupa la mesa
void Mesa::ocupar() {
    ocupada = true;
}
//Libera la mesa
void Mesa::liberar() {
    ocupada = false;
}

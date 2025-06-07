#include "Bebida.h"

Bebida::Bebida(std::string n, int t, double p) : nombre(n), tiempoPreparacion(t), precio(p) {}

double Bebida::obtenerPrecio() const {
    return precio;
}
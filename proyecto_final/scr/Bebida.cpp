#include "Bebida.h"

Bebida::Bebida(std::string n, int t, double p) : nombre(n), tiempoPreparacion(t), precio(p) {}

//Retorna el precio de la Bebida
double Bebida::obtenerPrecio() const {
    return precio;
}

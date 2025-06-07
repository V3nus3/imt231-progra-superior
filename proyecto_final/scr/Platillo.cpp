#include "Platillo.h"

Platillo::Platillo(std::string n, int t, double p) : nombre(n), tiempoPreparacion(t), precio(p) {}

double Platillo::obtenerPrecio() const {
    return precio;
}

int Platillo::obtenerTiempoPreparacion() const {
    return tiempoPreparacion;
}
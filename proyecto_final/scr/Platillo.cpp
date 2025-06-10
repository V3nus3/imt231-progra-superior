#include "Platillo.h"

Platillo::Platillo(std::string n, int t, double p) : nombre(n), tiempoPreparacion(t), precio(p) {}

//Obtiene el precio del platillo
double Platillo::obtenerPrecio() const {
    return precio;
}
//Obtiene el tiempo de preparacion del platillo
int Platillo::obtenerTiempoPreparacion() const {
    return tiempoPreparacion;
}

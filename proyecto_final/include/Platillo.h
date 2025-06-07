#ifndef PLATILLO_H
#define PLATILLO_H

#include <string>
#include <vector>

class Platillo {
public:
    std::string nombre;
    int tiempoPreparacion;
    double precio;
    std::vector<std::string> secuenciaElectrodomesticos;

    Platillo(std::string n, int t, double p);
    double obtenerPrecio() const;
    int obtenerTiempoPreparacion() const;
};

#endif // PLATILLO_H
#ifndef BEBIDA_H
#define BEBIDA_H

#include <string>
#include <vector>

class Bebida {
public:
    std::string nombre;
    int tiempoPreparacion;
    double precio;
    std::vector<std::string> secuenciaElectrodomesticos;

    Bebida(std::string n, int t, double p);
    double obtenerPrecio() const;
};

#endif // BEBIDA_H
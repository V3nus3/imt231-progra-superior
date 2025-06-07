#ifndef TRABAJADOR_H
#define TRABAJADOR_H

#include <string>

class Trabajador {
public:
    int idTrabajador;
    std::string nombre;
    std::string rol;

    Trabajador(int id, std::string n, std::string r);
    virtual ~Trabajador() = default;

    // Método virtual puro que las clases hijas deben implementar.
    virtual void trabajar() = 0;
};

#endif // TRABAJADOR_H
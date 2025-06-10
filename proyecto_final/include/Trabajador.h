#ifndef TRABAJADOR_H
#define TRABAJADOR_H

#include <string>
//Declaracion de la clase Padre Trabajador
class Trabajador {
public:
    int idTrabajador;
    std::string nombre;
    std::string rol;
//Constructor
    Trabajador(int id, std::string n, std::string r);
//Destructor virtual, se hace virtual para que las clases derivadas puedan sobreescribirlo por temas de
//dinamica
    virtual ~Trabajador() = default;

    // Método virtual puro que las clases hijas deben implementar.
    virtual void trabajar() = 0;
};

#endif // TRABAJADOR_H

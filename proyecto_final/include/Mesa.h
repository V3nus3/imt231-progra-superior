#ifndef MESA_H
#define MESA_H

#include <vector>
#include <mutex>
// #include "Cliente.h" // <-- LÍNEA ELIMINADA

class Cliente; // <-- LÍNEA AÑADIDA: Declaración adelantada

class Mesa {
public:
    int numeroMesa;
    int capacidad;
    bool ocupada;
    std::mutex mtx;

    Mesa(int num, int cap);
    bool estaDisponible() const;
    void ocupar();
    void liberar();
};

#endif // MESA_H
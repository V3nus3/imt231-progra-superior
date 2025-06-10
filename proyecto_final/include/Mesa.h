#ifndef MESA_H
#define MESA_H

#include <vector>
#include <mutex>

class Cliente;

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

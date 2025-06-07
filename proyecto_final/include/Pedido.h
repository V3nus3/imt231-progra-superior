#ifndef PEDIDO_H
#define PEDIDO_H

#include <vector>
#include <chrono>
#include "Platillo.h"
#include "Bebida.h"
#include "Global.h"
// #include "Cliente.h" // <-- LÍNEA ELIMINADA

class Cliente; // <-- LÍNEA AÑADIDA

class Pedido {
public:
    int idPedido;
    Cliente* cliente; // <-- Ahora es válido
    std::vector<Platillo> listaPlatillos;
    std::vector<Bebida> listaBebidas;
    EstadoPedido estado;
    std::chrono::system_clock::time_point tiempoCreacion;

    Pedido(int id, Cliente* c, const std::vector<Platillo>& p, const std::vector<Bebida>& b);
    double calcularTotal() const;
    void actualizarEstado(EstadoPedido nuevoEstado);
};

#endif // PEDIDO_H
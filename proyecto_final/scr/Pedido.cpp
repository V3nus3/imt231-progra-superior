#include "Pedido.h"

Pedido::Pedido(int id, Cliente* c, const std::vector<Platillo>& p, const std::vector<Bebida>& b)
    : idPedido(id), cliente(c), listaPlatillos(p), listaBebidas(b), estado(EstadoPedido::Pendiente) {
    tiempoCreacion = std::chrono::system_clock::now();
}

double Pedido::calcularTotal() const {
    double total = 0.0;
    for (const auto& platillo : listaPlatillos) {
        total += platillo.obtenerPrecio();
    }
    for (const auto& bebida : listaBebidas) {
        total += bebida.obtenerPrecio();
    }
    return total;
}

void Pedido::actualizarEstado(EstadoPedido nuevoEstado) {
    estado = nuevoEstado;
}
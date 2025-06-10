#include "Pedido.h"

//Constructor de Pedido
Pedido::Pedido(int id, Cliente* c, const std::vector<Platillo>& p, const std::vector<Bebida>& b)
    : idPedido(id), cliente(c), listaPlatillos(p), listaBebidas(b), estado(EstadoPedido::Pendiente) {
    tiempoCreacion = std::chrono::system_clock::now();
}
//Metodo que calcula el total sumando los precios del platillo y de la bebida
double Pedido::calcularTotal() const {
    double total = 0.0;
    for (const auto& platillo : listaPlatillos) {
        total += platillo.obtenerPrecio();
    }
    for (const auto& bebida : listaBebidas) {
        total += bebida.obtenerPrecio();
    }
    //retorno del total de la cuenta
    return total;
}
//Cambia de estado el Pedido
void Pedido::actualizarEstado(EstadoPedido nuevoEstado) {
    estado = nuevoEstado;
}

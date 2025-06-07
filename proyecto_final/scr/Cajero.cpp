#include "Cajero.h"
#include "Restaurante.h"
#include "Global.h"
#include "Pedido.h"

Cajero::Cajero(int id, std::string n, Restaurante* r)
    : Trabajador(id, n, "Cajero"), restaurante(r) {}

void Cajero::trabajar() {
    // La lógica del cajero es invocada por los clientes, no es un bucle activo.
}

Mesa* Cajero::asignarMesaACliente() {
    for (auto& mesa_ptr : restaurante->mesas) {
        std::lock_guard<std::mutex> lock(mesa_ptr->mtx);
        if (mesa_ptr->estaDisponible()) {
            mesa_ptr->ocupar();
            return mesa_ptr.get();
        }
    }
    return nullptr;
}

void Cajero::cobrarPedido(Pedido* pedido) {
    double total = pedido->calcularTotal();
    log("Cajero", nombre, "está cobrando el pedido", pedido->idPedido, "por un total de", total, "Bs.");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    pedido->actualizarEstado(EstadoPedido::Pagado);
    log("Pedido", pedido->idPedido, "ha sido PAGADO.");
}
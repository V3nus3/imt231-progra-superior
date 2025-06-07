#include "Mesero.h"
#include "Restaurante.h"
#include "Global.h"
#include "Pedido.h"   // <-- LÍNEA AÑADIDA
#include "Cliente.h"  // <-- LÍNEA AÑADIDA

Mesero::Mesero(int id, std::string n, Restaurante* r)
    : Trabajador(id, n, "Mesero"), restaurante(r) {}

void Mesero::trabajar() {
    log("Mesero", nombre, "ha iniciado su turno.");
    while (restaurante->simulacionActiva) {
        Pedido* pedido;
        {
            std::unique_lock<std::mutex> lock(restaurante->mtxPedidosListos);
            // Espera hasta que haya un pedido listo o la simulación termine.
            restaurante->cvPedidosListos.wait(lock, [this] {
                return !restaurante->pedidosListos.empty() || !restaurante->simulacionActiva;
            });

            if (!restaurante->simulacionActiva && restaurante->pedidosListos.empty()) {
                break;
            }

            pedido = restaurante->pedidosListos.front();
            restaurante->pedidosListos.pop();
        }
        entregarPedido(pedido);
    }
    log("Mesero", nombre, "ha terminado su turno.");
}

void Mesero::entregarPedido(Pedido* pedido) {
    // Estas líneas ahora son válidas
    log("Mesero", nombre, "está recogiendo el pedido", pedido->idPedido);
    std::this_thread::sleep_for(std::chrono::seconds(2)); 
    
    pedido->actualizarEstado(EstadoPedido::Entregado);
    log("Mesero", nombre, "ha ENTREGADO el pedido", pedido->idPedido, "a", pedido->cliente->nombre, "en la mesa", pedido->cliente->mesaAsignada->numeroMesa);
}
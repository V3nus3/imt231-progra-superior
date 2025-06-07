#include "Cocinero.h"
#include "Restaurante.h"
#include "Global.h"
#include <thread>
#include "Pedido.h"
#include "Cliente.h" // <-- LÍNEA AÑADIDA PARA LA CORRECCIÓN FINAL

Cocinero::Cocinero(int id, std::string n, Restaurante* r)
    : Trabajador(id, n, "Cocinero"), restaurante(r) {}

void Cocinero::trabajar() {
    log("Cocinero", nombre, "ha iniciado su turno.");
    while (restaurante->simulacionActiva) {
        Pedido* pedido;
        {
            std::unique_lock<std::mutex> lock(restaurante->mtxPedidosPendientes);
            restaurante->cvPedidosPendientes.wait(lock, [this] {
                return !restaurante->pedidosPendientes.empty() || !restaurante->simulacionActiva;
            });

            if (!restaurante->simulacionActiva && restaurante->pedidosPendientes.empty()) {
                break;
            }

            pedido = restaurante->pedidosPendientes.front();
            restaurante->pedidosPendientes.pop();
        }
        prepararPedido(pedido);
    }
    log("Cocinero", nombre, "ha terminado su turno.");
}

void Cocinero::prepararPedido(Pedido* pedido) {
    // Esta línea ahora es válida porque incluimos "Cliente.h"
    log("Cocinero", nombre, "comienza a preparar el pedido", pedido->idPedido, "para", pedido->cliente->nombre);
    pedido->actualizarEstado(EstadoPedido::EnPreparacion);

    // Preparar platillos
    for (const auto& platillo : pedido->listaPlatillos) {
        log("Cocinero", nombre, "preparando platillo:", platillo.nombre);
        std::this_thread::sleep_for(std::chrono::seconds(platillo.tiempoPreparacion / 4)); 
    }
    // Preparar bebidas
    for (const auto& bebida : pedido->listaBebidas) {
        log("Cocinero", nombre, "preparando bebida:", bebida.nombre);
        std::this_thread::sleep_for(std::chrono::seconds(bebida.tiempoPreparacion / 4));
    }

    pedido->actualizarEstado(EstadoPedido::Listo);
    log("Cocinero", nombre, "terminó el pedido", pedido->idPedido, ". Estado:", estadoToString(pedido->estado));

    {
        std::lock_guard<std::mutex> lock(restaurante->mtxPedidosListos);
        restaurante->pedidosListos.push(pedido);
    }
    restaurante->cvPedidosListos.notify_one(); 
}
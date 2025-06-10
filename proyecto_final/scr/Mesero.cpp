#include "Mesero.h"
#include "Restaurante.h"
#include "Global.h"
#include "Pedido.h" 
#include "Cliente.h"

//Constructor de Mesero
Mesero::Mesero(int id, std::string n, Restaurante* r)
    : Trabajador(id, n, "Mesero"), restaurante(r) {}

//Metodo Trabajar
void Mesero::trabajar() {
    //Registra que un mesero inicio su turno y lo muestra en la terminal
    log("Mesero", nombre, "ha iniciado su turno.");
    //bucle para asegurar que el mesero trabaje hasta que la simulacion termine
    while (restaurante->simulacionActiva) {
        Pedido* pedido;
        {
            //asegura que el mesero tenga acceso exclusivo a la cola de pedidos listos
            std::unique_lock<std::mutex> lock(restaurante->mtxPedidosListos);
            // Espera hasta que haya un pedido listo o la simulación termine
            //En caso de no haber un pedido listo espera a que haya un cambio
            restaurante->cvPedidosListos.wait(lock, [this] {
                return !restaurante->pedidosListos.empty() || !restaurante->simulacionActiva;
            });
            //Si la simulacion no esta activa y no hay pedidos listos, el mesero termina su turno y sale del bucle
            if (!restaurante->simulacionActiva && restaurante->pedidosListos.empty()) {
                break;
            }
            //Obtiene el primer pedido de la cola de pedidos listos
            pedido = restaurante->pedidosListos.front();
            //Elimina el pedido que fue tomado por el mesero
            restaurante->pedidosListos.pop();
        }
        //simula la entrega del pedido
        entregarPedido(pedido);
    }
    //Registra que el mesero termino su turno
    log("Mesero", nombre, "ha terminado su turno.");
}
//Metodo de entrega de pedido
void Mesero::entregarPedido(Pedido* pedido) {
    // Registra que el mesero recogio el pedido
    log("Mesero", nombre, "está recogiendo el pedido", pedido->idPedido);
    std::this_thread::sleep_for(std::chrono::seconds(2)); 
    //actualiza el estado del pedido a entregado
    pedido->actualizarEstado(EstadoPedido::Entregado);
    //Registra que el mesero entrego el pedido
    log("Mesero", nombre, "ha ENTREGADO el pedido", pedido->idPedido, "a", pedido->cliente->nombre, "en la mesa", pedido->cliente->mesaAsignada->numeroMesa);
}

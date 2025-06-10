#include "Cajero.h"
#include "Restaurante.h"
#include "Global.h"
#include "Pedido.h"

//Constructor de Cajero
Cajero::Cajero(int id, std::string n, Restaurante* r)
    : Trabajador(id, n, "Cajero"), restaurante(r) {}

//Metodo Trabajar
void Cajero::trabajar() {
    //El cajero no trabaja de manera activa , este es invocado cuando un cliente interactua con el
}
//Metodo Asignar Mesa
Mesa* Cajero::asignarMesaACliente() {
    //Bucle que recorre todas las mesas
    for (auto& mesa_ptr : restaurante->mesas) {
        //asegura que la mesa no sea modifica por otro hilo en lo que se verifica si esta disponible
        std::lock_guard<std::mutex> lock(mesa_ptr->mtx);
        //Verifica si la mesa esta disponible
        if (mesa_ptr->estaDisponible()) {
            //Ocupa la mesa
            mesa_ptr->ocupar();
            //Devueleve un puntero de la mesa que se acaba de ocupar
            return mesa_ptr.get();
        }
    }
    //En caso de no pillar mesa disponible envia un puntero nulo
    return nullptr;
}
//Metodo Cobrar Pedido
void Cajero::cobrarPedido(Pedido* pedido) {
    //Calcula el total del pedido mediante el metodo calcularTotal de la clase pedido
    double total = pedido->calcularTotal();
    //Registra en la terminal que el cajero esta cobrando el pedido segun el total
    log("Cajero", nombre, "está cobrando el pedido", pedido->idPedido, "por un total de", total, "Bs.");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    pedido->actualizarEstado(EstadoPedido::Pagado);
    //Registra que el pedido n se ha pagado
    log("Pedido", pedido->idPedido, "ha sido PAGADO.");
}

#include "Cliente.h"
#include "Restaurante.h"
#include "Global.h"
#include <thread>
#include "Cajero.h" 
#include "Pedido.h" 
#include "Mesa.h"   

//Constructor de Cliente
Cliente::Cliente(std::string n, Restaurante* r, const Platillo& p, const Bebida& b)
    : nombre(n), mesaAsignada(nullptr), pedidoActual(nullptr), restaurante(r), platilloDeseado(p), bebidaDeseada(b) {}
//Metodo esperar
void Cliente::esperando() {
    //Registra la llegada del cliente al restaurante
    log("Cliente", nombre, "ha llegado al restaurante.");
    //puntero al cajero del restaurante
    Cajero* cajero = restaurante->obtenerCajero();
    //En caso de no haber un cajero disponible el cliente se va
    if (!cajero) {
        //Registra si el cliente no encontro un cajero y se retira
        log("Cliente", nombre, "no encontró un cajero y se fue.");
        return;
    }

    {
        //Bloqueo de acceso a la caja del cajero para que otro hilo no acceda al mismo tiempo
        std::lock_guard<std::mutex> lock(cajero->caja_mtx);
        //registra que el cliente esta en caja y va a pagar el pedido
        log("Cliente", nombre, "está en la caja con", cajero->nombre);
        hacerPedido();
        cajero->cobrarPedido(pedidoActual);
    } 

    log("Cliente", nombre, "ha pagado y ahora busca una mesa.");
    //Proceso de busqueda de mesa y en caso de no encontrar espera a que una se libere y vuelve a intentarlo en 3 segundos
    while (mesaAsignada == nullptr) {
        mesaAsignada = cajero->asignarMesaACliente();
        if (mesaAsignada == nullptr) {
            log("Cliente", nombre, "está esperando por una mesa disponible...");
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    }
    log("Cliente", nombre, "se ha sentado en la mesa", mesaAsignada->numeroMesa);

    {
        //EL pedido del cliente se coloca en una cola de pedidos pendientes y notifica
        // Cocinero mediante cvPedidosPendientes
        std::lock_guard<std::mutex> lock(restaurante->mtxPedidosPendientes);
        restaurante->pedidosPendientes.push(pedidoActual);
    }
    restaurante->cvPedidosPendientes.notify_one();
    
    while (pedidoActual->estado != EstadoPedido::Entregado) {
        //EL clientes espero a que cambie el estado de su pedido a entregado, mientras
        // duerme en intervalos de 500 milisegundos
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
//una vez el estado del pedido del cliente cambia a entregado empieza a consumir su pedido
    consumirPedido();
//registra que el cliente termino su comida y se fue
    log("Cliente", nombre, "terminó de comer y se retira.");
    {
        std::lock_guard<std::mutex> lock(mesaAsignada->mtx);
        mesaAsignada->liberar();
    }
    //Registro de quje la mesa del cliente se desocupo
    log("Mesa", mesaAsignada->numeroMesa, "ha sido liberada.");
    //se elimina el pedido actual
    delete pedidoActual;
}

void Cliente::hacerPedido() {
    //Incrementea el identificador del proximo pedido
    int id = restaurante->proximoIdPedido++;
    std::vector<Platillo> platillos = {platilloDeseado};
    std::vector<Bebida> bebidas = {bebidaDeseada};
    pedidoActual = new Pedido(id, this, platillos, bebidas);
    log("Cliente", nombre, "ha creado el pedido", id, "con", platilloDeseado.nombre, "y", bebidaDeseada.nombre);
}
//Metodo cosumir pedido
void Cliente::consumirPedido() {
    //se registra que el cliente esta consumiendo su pedido
    log("Cliente", nombre, "está consumiendo su pedido. ¡Buen provecho!");
    //se le asigna un tiempo de consumo de 10 segundos
    std::this_thread::sleep_for(std::chrono::seconds(10));
}

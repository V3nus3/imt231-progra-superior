#include "Cliente.h"
#include "Restaurante.h"
#include "Global.h"
#include <thread>
#include "Cajero.h" 
#include "Pedido.h" 
#include "Mesa.h"   

// ... El constructor y la función esperando() no cambian ...

Cliente::Cliente(std::string n, Restaurante* r, const Platillo& p, const Bebida& b)
    : nombre(n), mesaAsignada(nullptr), pedidoActual(nullptr), restaurante(r), platilloDeseado(p), bebidaDeseada(b) {}

void Cliente::esperando() {
    log("Cliente", nombre, "ha llegado al restaurante.");
    
    Cajero* cajero = restaurante->obtenerCajero();
    if (!cajero) {
        log("Cliente", nombre, "no encontró un cajero y se fue.");
        return;
    }

    {
        std::lock_guard<std::mutex> lock(cajero->caja_mtx);
        log("Cliente", nombre, "está en la caja con", cajero->nombre);
        hacerPedido();
        cajero->cobrarPedido(pedidoActual);
    } 

    log("Cliente", nombre, "ha pagado y ahora busca una mesa.");
    
    while (mesaAsignada == nullptr) {
        mesaAsignada = cajero->asignarMesaACliente();
        if (mesaAsignada == nullptr) {
            log("Cliente", nombre, "está esperando por una mesa disponible...");
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    }
    log("Cliente", nombre, "se ha sentado en la mesa", mesaAsignada->numeroMesa);

    {
        std::lock_guard<std::mutex> lock(restaurante->mtxPedidosPendientes);
        restaurante->pedidosPendientes.push(pedidoActual);
    }
    restaurante->cvPedidosPendientes.notify_one();
    
    while (pedidoActual->estado != EstadoPedido::Entregado) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    consumirPedido();

    log("Cliente", nombre, "terminó de comer y se retira.");
    {
        std::lock_guard<std::mutex> lock(mesaAsignada->mtx);
        mesaAsignada->liberar();
    }
    log("Mesa", mesaAsignada->numeroMesa, "ha sido liberada.");
    delete pedidoActual;
}

void Cliente::hacerPedido() {
    int id = restaurante->proximoIdPedido++;
    // CORRECCIÓN: Crear los vectores explícitamente
    std::vector<Platillo> platillos = {platilloDeseado};
    std::vector<Bebida> bebidas = {bebidaDeseada};
    pedidoActual = new Pedido(id, this, platillos, bebidas);
    log("Cliente", nombre, "ha creado el pedido", id, "con", platilloDeseado.nombre, "y", bebidaDeseada.nombre);
}

void Cliente::consumirPedido() {
    log("Cliente", nombre, "está consumiendo su pedido. ¡Buen provecho!");
    std::this_thread::sleep_for(std::chrono::seconds(10));
}
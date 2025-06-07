#ifndef RESTAURANTE_H
#define RESTAURANTE_H

#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <memory>
#include "Trabajador.h"
#include "Platillo.h"
#include "Bebida.h"
#include "Electrodomestico.h"
#include "Mesa.h"

// Declaraciones adelantadas
class Cajero;
class Cocinero;
class Mesero;
class Pedido;

class Restaurante {
private:
    void cargarDatos();

public:
    // ---- RECURSOS DEL RESTAURANTE ----
    std::vector<std::unique_ptr<Mesa>> mesas;
    std::vector<std::unique_ptr<Trabajador>> trabajadores;
    std::vector<Platillo> menuPlatillos;
    std::vector<Bebida> menuBebidas;
    // CORRECCIÓN FINAL: Cambiado a un vector de punteros inteligentes
    std::vector<std::unique_ptr<Electrodomestico>> electrodomesticos;

    // ... el resto del archivo sin cambios ...
    std::queue<Pedido*> pedidosPendientes;
    std::mutex mtxPedidosPendientes;
    std::condition_variable cvPedidosPendientes;
    std::queue<Pedido*> pedidosListos;
    std::mutex mtxPedidosListos;
    std::condition_variable cvPedidosListos;
    std::atomic<bool> simulacionActiva;
    std::atomic<int> proximoIdPedido;

    Restaurante();
    ~Restaurante();

    void iniciarSimulacion();

    Cajero* obtenerCajero();
    Electrodomestico* obtenerElectrodomestico(const std::string& tipo);
    Platillo obtenerPlatilloPorNombre(const std::string& nombre);
    Bebida obtenerBebidaPorNombre(const std::string& nombre);
};

#endif // RESTAURANTE_H
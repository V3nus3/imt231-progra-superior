#ifndef RESTAURANTE_H
#define RESTAURANTE_H

#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
//Manejo de hilos, debido a la concurrencia que el restaurante podra usar para la simulacion de pedidos y preparacion
#include <thread>
//Manejo de variables atomicas para controlar identificadores de los pedidos sin que los hilos se interfieran entre si
#include <atomic>
//Uso de punteros inteligentes para la gestion automatica de memoria
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

//Lista de punteros inteligentes a la lista de mesas
    std::vector<std::unique_ptr<Mesa>> mesas;
//Lsita de punteros inteligentes al Objeto Trabajador representando a los empleados del restaurante
    std::vector<std::unique_ptr<Trabajador>> trabajadores;
//Lista de punteros inteligentes a los platillos disponibles
    std::vector<Platillo> menuPlatillos;
//Lista de punteros inteligentes a las bebidas disponibles
    std::vector<Bebida> menuBebidas;
//Lista de punteros inteligentes al Objeto Electrodomestico que representa a la lista de electrodomesticos disponibles para la preparacion de platillos
    std::vector<std::unique_ptr<Electrodomestico>> electrodomesticos;
//Cola que almacena pedidos pendientes
    std::queue<Pedido*> pedidosPendientes;
//acceso a la cola de pedidos pendientes entre multiples hilos
    std::mutex mtxPedidosPendientes;
//condicion para notificar a los hilos en espera cuando hay uno nuevo en la cola
    std::condition_variable cvPedidosPendientes;
//Cola de pedidos listos para entregar
    std::queue<Pedido*> pedidosListos;
//sincronizar acceso a cola de pedidos listos
    std::mutex mtxPedidosListos;
//condicion para notificar a los hilos en espera cuando hay un nuevo pedido listo
    std::condition_variable cvPedidosListos;
//Valor atomico para indicar si la simulacion esta activa, usada para controlar el ciclo de la simulacion
    std::atomic<bool> simulacionActiva;
//Contador atomico que asigna un Id unico a cada nuevo pedido
    std::atomic<int> proximoIdPedido;

    Restaurante();
    ~Restaurante();

    void iniciarSimulacion();

//Metodos para obtener datos del restaurante
    Cajero* obtenerCajero();
    Electrodomestico* obtenerElectrodomestico(const std::string& tipo);
    Platillo obtenerPlatilloPorNombre(const std::string& nombre);
    Bebida obtenerBebidaPorNombre(const std::string& nombre);
};

#endif // RESTAURANTE_H

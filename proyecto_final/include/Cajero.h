#ifndef CAJERO_H
#define CAJERO_H

#include "Trabajador.h"
#include <mutex> // <-- LÍNEA AÑADIDA

// Declaraciones adelantadas para resolver dependencias
class Restaurante;
class Mesa;
class Pedido;

class Cajero : public Trabajador {
public:
    Restaurante* restaurante;
    std::mutex caja_mtx; // <-- LÍNEA AÑADIDA: Mutex para la caja

    Cajero(int id, std::string n, Restaurante* r);
    void trabajar() override;
    Mesa* asignarMesaACliente();
    void cobrarPedido(Pedido* pedido);
};

#endif // CAJERO_H
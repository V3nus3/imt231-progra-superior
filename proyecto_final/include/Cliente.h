#ifndef CLIENTE_H
#define CLIENTE_H

#include <string>
// CORRECCIÓN: Incluir las definiciones completas en lugar de declaraciones adelantadas
#include "Platillo.h"
#include "Bebida.h"

// Declaraciones adelantadas para los punteros
class Mesa;
class Pedido;
class Restaurante;

class Cliente {
public:
    std::string nombre;
    Mesa* mesaAsignada;
    Pedido* pedidoActual;
    Restaurante* restaurante;
    Platillo platilloDeseado; // <-- Ahora esto es válido
    Bebida bebidaDeseada;   // <-- Ahora esto es válido

    Cliente(std::string n, Restaurante* r, const Platillo& p, const Bebida& b);
    void esperando();
    void hacerPedido();
    void consumirPedido();
};

#endif // CLIENTE_H
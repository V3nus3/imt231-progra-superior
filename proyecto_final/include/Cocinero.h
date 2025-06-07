#ifndef COCINERO_H
#define COCINERO_H

#include "Trabajador.h"
#include "Restaurante.h"

class Restaurante; // Declaración adelantada

class Cocinero : public Trabajador {
public:
    Restaurante* restaurante;

    Cocinero(int id, std::string n, Restaurante* r);
    void trabajar() override;
    void prepararPedido(Pedido* pedido);
};

#endif // COCINERO_H
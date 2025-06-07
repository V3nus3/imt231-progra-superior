#ifndef MESERO_H
#define MESERO_H

#include "Trabajador.h"
#include "Restaurante.h"

class Restaurante; // Declaración adelantada

class Mesero : public Trabajador {
public:
    Restaurante* restaurante;

    Mesero(int id, std::string n, Restaurante* r);
    void trabajar() override;
    void entregarPedido(Pedido* pedido);
};

#endif // MESERO_H
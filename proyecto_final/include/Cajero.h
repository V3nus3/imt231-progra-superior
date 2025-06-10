#ifndef CAJERO_H
#define CAJERO_H

#include "Trabajador.h"
#include <mutex> 

// Se Declaran estas clases para resolver dependecias que tendra cajero
class Restaurante;
class Mesa;
class Pedido;

class Cajero : public Trabajador { //Aqui la clase Cajero hereda del padre Trabajador
public:
    Restaurante* restaurante;
    std::mutex caja_mtx; // Se usa el mutex para bloquear el hilo caja hasta que este desocupe

    Cajero(int id, std::string n, Restaurante* r);
    void trabajar() override;
    Mesa* asignarMesaACliente();
    void cobrarPedido(Pedido* pedido);
};

#endif // CAJERO_H

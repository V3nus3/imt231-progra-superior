#ifndef PEDIDO_H
#define PEDIDO_H

#include <vector>
#include <chrono>
#include "Platillo.h"
#include "Bebida.h"
#include "Global.h"

class Cliente;

class Pedido {
public:
    int idPedido;
    Cliente* cliente; 
    std::vector<Platillo> listaPlatillos;
    std::vector<Bebida> listaBebidas;
    EstadoPedido estado;
//Se crea un objeto tipo time_point el cual guarda el momento exactoq ue se creo el pedido esto con fines de
// calcular tiempos de preparacion de platillos o bebidas
    std::chrono::system_clock::time_point tiempoCreacion;
//COnstructor de Pedido
    Pedido(int id, Cliente* c, const std::vector<Platillo>& p, const std::vector<Bebida>& b);
//Calcula total del pedido
    double calcularTotal() const;
//Actualizacion del estado del pedido a un nuevo valor
    void actualizarEstado(EstadoPedido nuevoEstado);
};

#endif // PEDIDO_H

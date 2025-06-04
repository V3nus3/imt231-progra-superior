#ifndef CLIENTE_HPP
#define CLIENTE_HPP

#include <string>
#include <vector>
#include <iostream>
#include <json.hpp>

class Cliente {
private:
    std::string nombre;
    int id_cliente;
    int nro_mesa;
    std::string pedido_platillo;
    std::string pedido_bebida;
    float cuenta_Total;
    bool atendido;

public:
    Cliente();
    Cliente(std::string nombre, int id, int mesa, std::string platillo, std::string bebida);

    // Getters y setters
    std::string getNombre() const;
    int getId() const;
    int getMesa() const;
    std::string getPlatillo() const;
    std::string getBebida() const;
    float getCuenta() const;
    bool estaAtendido() const;

    void setCuenta(float cuenta);
    void marcarAtendido();

    // Métodos funcionales
    void AsignarMesa(int nroMesa);
    void HacerPedido(std::string platillo, std::string bebida);
    void PagarCuenta();

    // Lectura de archivo JSON
    static std::vector<Cliente> cargarClientesDesdeJSON(const std::string& archivo);

    void mostrarInfo() const;
};

#endif


#include <fstream>
#include "../include/json.hpp"
#include "../include/cliente.h"
using json = nlohmann::json;

// Constructores
Cliente::Cliente() : id_cliente(0), nro_mesa(-1), cuenta_Total(0.0), atendido(false) {}

Cliente::Cliente(std::string nombre, int id, int mesa, std::string platillo, std::string bebida)
    : nombre(nombre), id_cliente(id), nro_mesa(mesa),
      pedido_platillo(platillo), pedido_bebida(bebida), cuenta_Total(0.0), atendido(false) {}

// Getters
std::string Cliente::getNombre() const { return nombre; }
int Cliente::getId() const { return id_cliente; }
int Cliente::getMesa() const { return nro_mesa; }
std::string Cliente::getPlatillo() const { return pedido_platillo; }
std::string Cliente::getBebida() const { return pedido_bebida; }
float Cliente::getCuenta() const { return cuenta_Total; }
bool Cliente::estaAtendido() const { return atendido; }

// Setters
void Cliente::setCuenta(float cuenta) { cuenta_Total = cuenta; }
void Cliente::marcarAtendido() { atendido = true; }

// Métodos de acción
void Cliente::AsignarMesa(int nroMesa) {
    nro_mesa = nroMesa;
}

void Cliente::HacerPedido(std::string platillo, std::string bebida) {
    pedido_platillo = platillo;
    pedido_bebida = bebida;
}

void Cliente::PagarCuenta() {
    std::cout << "El cliente " << nombre << " ha pagado: Bs " << cuenta_Total << std::endl;
    cuenta_Total = 0.0;
    atendido = true;
}

// Mostrar información
void Cliente::mostrarInfo() const {
    std::cout << "Cliente: " << nombre << " | ID: " << id_cliente
              << " | Mesa: " << nro_mesa << " | Platillo: " << pedido_platillo
              << " | Bebida: " << pedido_bebida << " | Cuenta: " << cuenta_Total
              << " | Atendido: " << (atendido ? "Sí" : "No") << std::endl;
}

// Cargar lista de clientes desde archivo JSON
std::vector<Cliente> Cliente::cargarClientesDesdeJSON(const std::string& archivo) {
    std::ifstream entrada(archivo);
    if (!entrada.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo JSON de clientes.");
    }

    json datos_json;
    entrada >> datos_json;

    std::vector<Cliente> clientes;
    int id = 1;
    for (const auto& item : datos_json) {
        Cliente c(
            item["nombre"],
            id++,
            -1, // Sin mesa asignada aún
            item["platillo"],
            item["bebida"]
        );
        clientes.push_back(c);
    }

    return clientes;
}

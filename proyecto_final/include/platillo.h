#ifndef PLATILLO_H
#define PLATILLO_H

#include <string>
#include <vector>
#include <iostream>

class Platillo {
private:
    std::string nombre;
    float precio;
    int tiempo_preparacion; // en minutos
    std::vector<std::string> ingredientes;

    int id_cliente_asignado;
    std::string electrodomestico_usado;

public:
    // Constructores
    Platillo();
    Platillo(std::string nombre, float precio, int tiempo, std::vector<std::string> ingredientes);

    // Getters
    std::string getNombre() const;
    float getPrecio() const;
    int getTiempoPreparacion() const;
    std::vector<std::string> getIngredientes() const;

    // Métodos funcionales
    void AsignarCliente(int id_cliente);
    void UsoElectrodomestico(const std::string& electrodomestico);
    void MostrarInfo() const;
};

#endif

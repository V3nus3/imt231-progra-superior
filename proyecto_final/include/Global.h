#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <string>
#include <mutex>
#include <chrono>
#include <iomanip>

//Creacioon de la clase EstaodPedido para facilitar el manejo de estados del Pedido
//define los posibles estados de un pedido
enum class EstadoPedido {
    Pendiente,
    Pagado,
    EnPreparacion,
    Listo,
    Entregado
};

// Convierte EstadoPedido a un string de forma que este se pueda imprimir en la terminal
inline std::string estadoToString(EstadoPedido estado) {
    switch (estado) {
        case EstadoPedido::Pendiente: return "Pendiente";
        case EstadoPedido::Pagado: return "Pagado";
        case EstadoPedido::EnPreparacion: return "En Preparacion";
        case EstadoPedido::Listo: return "LISTO para servir";
        case EstadoPedido::Entregado: return "Entregado";
        default: return "Desconocido";
    }
}

// Mutex global para sincronizar a la terminal de salida y los mensajes de salida no se mezclen entre si
//para asi evitar la convolucion de mensajes de salida
inline std::mutex logMutex;

// Función log imprimira los mensaje de salida en la terminal de forma segura en un entorno multihilo
template<typename... Args>
void log(Args&&... args) {
    //Se asegura que un solo hilo a la vez, ejecute el bloque de codigo que imprime en la terminal
    std::lock_guard<std::mutex> lock(logMutex);
    // Imprime la marca de tiempo para cada evento.
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::cout << std::put_time(std::localtime(&in_time_t), "[%H:%M:%S] ");
    // Imprime los argumentos del mensaje.
    ((std::cout << std::forward<Args>(args) << ' '), ...);
    std::cout << std::endl;
}

#endif // GLOBAL_H

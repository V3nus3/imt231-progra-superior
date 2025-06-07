#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <string>
#include <mutex>
#include <chrono>
#include <iomanip>

// Enum para el estado de los pedidos, requerido por múltiples clases.
enum class EstadoPedido {
    Pendiente,
    Pagado,
    EnPreparacion,
    Listo,
    Entregado
};

// Convierte el enum a string para poder imprimirlo.
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

// Mutex global para sincronizar la salida a la consola y evitar texto mezclado.
inline std::mutex logMutex;

// Función de registro segura para hilos.
template<typename... Args>
void log(Args&&... args) {
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
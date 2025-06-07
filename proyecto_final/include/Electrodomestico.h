#ifndef ELECTRODOMESTICO_H
#define ELECTRODOMESTICO_H

#include <string>
#include <mutex>

class Electrodomestico {
public:
    std::string tipo;
    std::mutex mtx;

    Electrodomestico(std::string t);
    void usar();
    void liberar();
};

#endif // ELECTRODOMESTICO_H
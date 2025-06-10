#include "Electrodomestico.h"

Electrodomestico::Electrodomestico(std::string t) : tipo(t) {}

//Bloqueo o pone en estado ocupado el Electrodomestrico
void Electrodomestico::usar() {
    mtx.lock();
}
//Libera el Electrodomestico
void Electrodomestico::liberar() {
    mtx.unlock();
}

#include "Electrodomestico.h"

Electrodomestico::Electrodomestico(std::string t) : tipo(t) {}

void Electrodomestico::usar() {
    mtx.lock();
}

void Electrodomestico::liberar() {
    mtx.unlock();
}
#include "restaurante.h"

int main() {
    // Se crea el objeto Restaurante, que carga toda la configuración en su constructor.
    Restaurante miRestaurante;

    // Se inicia la simulación, que gestiona los hilos de trabajadores y clientes.
    miRestaurante.iniciarSimulacion();

    return 0;
}
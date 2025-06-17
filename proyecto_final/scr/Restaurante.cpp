#include "Restaurante.h"
#include "Global.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <stdexcept>
#include "Cajero.h"
#include "Cocinero.h"
#include "Mesero.h"
#include "Cliente.h"

using json = nlohmann::json;

//Constructor de Restaurante
Restaurante::Restaurante() : simulacionActiva(true), proximoIdPedido(1) {
    try {
        //Cargar los datos de platillos, bebidas, trabajadores, electrodomesticos desde los archivos JSON
        cargarDatos();
    } catch (const std::exception& e) {
        log("ERROR CRÍTICO al cargar datos:", e.what());
        simulacionActiva = false;
    }
}
//Destrucotr de la clase restaurante
Restaurante::~Restaurante() {}
//Metodo cargar datos al restaurante
void Restaurante::cargarDatos() {
    //apertura de platillos.json
    std::ifstream f_platillos("data/platillos.json");
    if (!f_platillos.is_open()) throw std::runtime_error("No se pudo abrir platillos.json");
    json dataPlatillos = json::parse(f_platillos);
    //Busqueda y acceso a datos del platillo
    for (const auto& item : dataPlatillos["platillos"]) {
        menuPlatillos.emplace_back(item["nombre"], item["tiempo_preparacion_min"], item["precio"]);
    }
    //Busqueda de datos de la bebida
    for (const auto& item : dataPlatillos["bebidas"]) {
        menuBebidas.emplace_back(item["nombre"], item["tiempo_preparacion_min"], item["precio"]);
    }
    //Registra un mensaje en la terminal de la cantidad de platillos y bebidas fueron cargados
    log("Menú cargado:", menuPlatillos.size(), "platillos y", menuBebidas.size(), "bebidas.");

    //Lectura de datos de trabajadores
    std::ifstream f_trabajadores("data/trabajadores_list.json");
    if (!f_trabajadores.is_open()) throw std::runtime_error("No se pudo abrir trabajadores_list.json");
    json dataTrabajadores = json::parse(f_trabajadores);
    //Bucle para recorrer el archivo de trabajadores JSON
    for (const auto& item : dataTrabajadores) {
        //Compara el valor del rol del trabajador
        if (item["rol"] == "Cajero") {
            //crea un objeto cajero usando los datos del trabajador esto para todos los tipos de rol
            trabajadores.emplace_back(std::make_unique<Cajero>(item["id_trabajador"], item["nombre"], this));
        } else if (item["rol"] == "Cocinero") {
            trabajadores.emplace_back(std::make_unique<Cocinero>(item["id_trabajador"], item["nombre"], this));
        } else if (item["rol"] == "Mesero") {
            trabajadores.emplace_back(std::make_unique<Mesero>(item["id_trabajador"], item["nombre"], this));
        }
    }
    //Registra cuantos trbajadores fueron cargados correcatmente ala restaurante
    log("Personal cargado:", trabajadores.size(), "trabajadores.");

    //Lectura de datos de Electrodomestricos
    std::ifstream f_electro("data/electrodomesticos_list.json");
    //MArca error si no se logro abrir la lista de electrodomesticos
    if (!f_electro.is_open()) throw std::runtime_error("No se pudo abrir electrodomesticos_list.json");
    json dataElectro = json::parse(f_electro);
    //Recorre cada electrodomestrico en el JSON
    for (const auto& item : dataElectro) {
        electrodomesticos.emplace_back(std::make_unique<Electrodomestico>(item["tipo"].get<std::string>()));
    }
    //Registra el numero de Electrodomesticos en la cocina
    log("Cocina equipada con:", electrodomesticos.size(), "electrodomésticos.");
    
    for(int i = 1; i <= 10; ++i) {
        mesas.emplace_back(std::make_unique<Mesa>(i, 4));
    }
    //Registra la cantidad de mesas en el restaurantes
    log(mesas.size(), "mesas listas para los clientes.");
}

Cajero* Restaurante::obtenerCajero() {
    for (const auto& trab : trabajadores) {
        if (auto cajero = dynamic_cast<Cajero*>(trab.get())) {
            return cajero;
        }
    }
    return nullptr;
}

Electrodomestico* Restaurante::obtenerElectrodomestico(const std::string& tipo) {
    for (auto& electro_ptr : electrodomesticos) {
        if (electro_ptr->tipo == tipo) {
            return electro_ptr.get();
        }
    }
    return nullptr;
}

Platillo Restaurante::obtenerPlatilloPorNombre(const std::string& nombre) {
    for(const auto& p : menuPlatillos) {
        if (p.nombre == nombre) return p;
    }
    throw std::runtime_error("Platillo no encontrado: " + nombre);
}

Bebida Restaurante::obtenerBebidaPorNombre(const std::string& nombre) {
    for(const auto& b : menuBebidas) {
        if (b.nombre == nombre) return b;
    }
    throw std::runtime_error("Bebida no encontrada: " + nombre);
}

void Restaurante::iniciarSimulacion() {
    if (!simulacionActiva) {
        log("La simulación no puede iniciar debido a un error previo.");
        return;
    }
    log("=== INICIANDO SIMULACIÓN DEL RESTAURANTE ===");
    
    std::vector<std::thread> hilosTrabajadores;
    for (auto& trabajador : trabajadores) {
        if(trabajador) {
            hilosTrabajadores.emplace_back(&Trabajador::trabajar, trabajador.get());
        }
    }

    std::ifstream f_clientes("data/clientes_list.json");
    if (!f_clientes.is_open()) throw std::runtime_error("No se pudo abrir clientes_list.json");
    json dataClientes = json::parse(f_clientes);
    
    std::vector<std::thread> hilosClientes;
    auto inicioSimulacion = std::chrono::steady_clock::now();

    for (const auto& item : dataClientes) {
        float tiempo_llegada_seg = item["tiempo_llegada"];
        std::this_thread::sleep_until(inicioSimulacion + std::chrono::duration<float>(tiempo_llegada_seg));
        
        try {
            Platillo p = obtenerPlatilloPorNombre(item["platillo"]);
            Bebida b = obtenerBebidaPorNombre(item["bebida"]);
            
            hilosClientes.emplace_back([nombre = item["nombre"].get<std::string>(), this, p, b]() {
                Cliente c(nombre, this, p, b);
                c.esperando(); // <-- LLAMADA A LA FUNCIÓN RENOMBRADA
            });

        } catch(const std::runtime_error& e) {
            log("Error:", e.what(), "- El cliente", item["nombre"].get<std::string>(), "se va decepcionado.");
        }
    }

    for (auto& t : hilosClientes) {
        if (t.joinable()) {
            t.join();
        }
    }

    log("=== TODOS LOS CLIENTES HAN SIDO ATENDIDOS ===");
    log("Cerrando el restaurante...");

    simulacionActiva = false;
    cvPedidosPendientes.notify_all();
    cvPedidosListos.notify_all();

    for (auto& t : hilosTrabajadores) {
        if (t.joinable()) {
            t.join();
        }
    }
    
    log("=== SIMULACIÓN TERMINADA ===");
}

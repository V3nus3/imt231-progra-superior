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

// Este metodo devuelve un puntero al cejero del restaruante
Cajero* Restaurante::obtenerCajero() {
    for (const auto& trab : trabajadores) {
        //el dinamic cast es para buscar un trabajador de tipo cajero
        if (auto cajero = dynamic_cast<Cajero*>(trab.get())) {
            return cajero;
        }
    }
    return nullptr;
}

//Devuevle un puntero a un electrodomestico especifico por el tipo
Electrodomestico* Restaurante::obtenerElectrodomestico(const std::string& tipo) {
    for (auto& electro_ptr : electrodomesticos) {
        if (electro_ptr->tipo == tipo) {
            return electro_ptr.get();
        }
    }
    return nullptr;
}

//Busqueda de platillo por nombre y este lo devuelve
Platillo Restaurante::obtenerPlatilloPorNombre(const std::string& nombre) {
    for(const auto& p : menuPlatillos) {
        if (p.nombre == nombre) return p;
    }
    // En caso de no encontrar el platillo devuelve un mensaje
    throw std::runtime_error("Platillo no encontrado: " + nombre);
}
//Busqueda de bebida por nombre y este lo devuelve
Bebida Restaurante::obtenerBebidaPorNombre(const std::string& nombre) {
    for(const auto& b : menuBebidas) {
        if (b.nombre == nombre) return b;
    }
    // En caso de no encontrar la bebida devuelve un mensaje
    throw std::runtime_error("Bebida no encontrada: " + nombre);
}

void Restaurante::iniciarSimulacion() {
    //Se verifica si la simulacion esta activa
    if (!simulacionActiva) {
        //En caso de no estar activa manda un mensaje notificando este error
        log("La simulación no puede iniciar debido a un error previo.");
        return;
    }
    //en caso de si estar activa la simulacion se inicia el restaurante
    log("=== INICIANDO SIMULACIÓN DEL RESTAURANTE ===");

    //Se crea un vector de hilosTrabajadores
    std::vector<std::thread> hilosTrabajadores;
    //este bucle recorre el vector de trabajadores el cual contiene punteros inteligentes a
    // objeto trabajador
    for (auto& trabajador : trabajadores) {
        if(trabajador) {
            //Se crea un hilo por cada trabajador
            //emplace_back agrega un hilo al vector hilosTrabajadores
            //Trabajador::trabajar es para que cada hilo ejecute el metodo trabajar
            //trabajador.get obtiene el puntero crudo de la clase trabajador desde el puntero inteligente para pasar la referencia al hilo
            hilosTrabajadores.emplace_back(&Trabajador::trabajar, trabajador.get()); //Ayuda de chatgpt
        }
    }
//Se abre el archivo de clientes json
    std::ifstream f_clientes("data/clientes_list.json");
    //en caso de no poder abrirse la lista devuevele un mensaje de error
    if (!f_clientes.is_open()) throw std::runtime_error("No se pudo abrir clientes_list.json");
    //Chat GPT nos dijo que mejor creemos un objeto json que contenga los datos de los clientes
    json dataClientes = json::parse(f_clientes);
    //hilos clientes almacena los hilos de ejecucion de los clientes
    std::vector<std::thread> hilosClientes;
    // guarda el momento en el q inicia la simulacion, el cual se ocupaa para la sincronizar la llegada de los clientes
    auto inicioSimulacion = std::chrono::steady_clock::now();
//Itera sobre cada cliente en dataClientes
    for (const auto& item : dataClientes) {
        //Extrae el teimpo de llegada de cada cliente
        float tiempo_llegada_seg = item["tiempo_llegada"];
        //hace que el hilo de cada cliente espere en base a su tiempo de llegada
        std::this_thread::sleep_until(inicioSimulacion + std::chrono::duration<float>(tiempo_llegada_seg));
        
        try {
            //Estos dos metodos se obtiene el platillo y la bebida que desea el cleinte
            Platillo p = obtenerPlatilloPorNombre(item["platillo"]);
            Bebida b = obtenerBebidaPorNombre(item["bebida"]);
            //En caso de no encontrar uno de estos dos lanzara un mensaje

            //Se crea un hilo para cada cliente para agregar un nuevo hilo al vector hilosClientes.
            //Chat GPT nos ayudo con esta parte de codigo
            hilosClientes.emplace_back([nombre = item["nombre"].get<std::string>(), this, p, b]() {
                //Aqui se crea un objeto cliente con el nombre del cliente, el platillo deseado y la bebida
                Cliente c(nombre, this, p, b);
                c.esperando();
            });
        //en caso de que hara un error al obtener el platillo o la bebida del cliente se registra un mensaje.
        } catch(const std::runtime_error& e) {
            log("Error:", e.what(), "- El cliente", item["nombre"].get<std::string>(), "se va decepcionado.");
        }
    }
//Espera a q los clientes terminen
    for (auto& t : hilosClientes) {
        if (t.joinable()) {
            //el t.join hace que el hilo principal espere a que el hilo del cliente termine antes de continuar
            //para asegurar que todos los cliente hayan sido atendidos antes de terminar la simulacion
            t.join();
        }
    }

    log("=== TODOS LOS CLIENTES HAN SIDO ATENDIDOS ===");
    log("Cerrando el restaurante...");

    //Desactiva la simulacion
    simulacionActiva = false;
    //Notifica a todos los hilos que la simulacion termino y que pueden cerrar sus tareas
    cvPedidosPendientes.notify_all();
    cvPedidosListos.notify_all();

    for (auto& t : hilosTrabajadores) {
        //Verifica si el hilo puede ser unido 
        if (t.joinable()) {
            //espera a q todos los hilos trbajadores terminen su ejecucion ates de continuar
            t.join();
        }
    }
    
    log("=== SIMULACIÓN TERMINADA ===");
}

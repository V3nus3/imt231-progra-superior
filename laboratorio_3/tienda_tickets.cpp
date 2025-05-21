// tienda_tickets.cpp

#include "tienda_tickets.h"
#include "globals.h" 
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip> // para usar std::fixed and std::setprecision
#include <cmath>   // para std::round
#include <mutex>
#include <condition_variable>
#include <numeric> //  std::iota
#include <algorithm> // std::sort

//variables globales para la sincronización y el estado
//estas variables son compartidas entre todos los hilos
//y se utilizan para gestionar el estado de la tienda y los clientes
std::mutex mtx;
std::condition_variable cv_tienda;
std::condition_variable cv_vip_entrada; 
std::condition_variable cv_normal_entrada; 

int clientes_dentro = 0;
int vips_esperando = 0;
int vips_dentro = 0;
double ganancias_totales = 0.0;

// Timestamp inicial (al cargar el módulo)
static const auto TIEMPO_INICIAL = std::chrono::steady_clock::now();

// Devuelve los segundos transcurridos desde el arranque, redondeados a 1 decimal
double get_segundos_transcurridos() {
    auto ahora = std::chrono::steady_clock::now();
    std::chrono::duration<double> dif = ahora - TIEMPO_INICIAL;
    double segs = dif.count();
    return std::round(segs * 10.0) / 10.0;
}
//ejecuta la funcion por cada cliente
static void cliente_hilo(const Cliente cliente_actual, double precio_ticket, int capacidad_maxima) {
    register_thread(cliente_actual.nombre);

    // 1. Esperar retrasoIngreso segundos
    std::this_thread::sleep_for(std::chrono::duration<double>(cliente_actual.retrasoIngreso));
//
    std::unique_lock<std::mutex> lock(mtx);

    // maneja VIP queue
    if (cliente_actual.VIP) {
        vips_esperando++;
        // VIPs esperan si la tienda está llena o si hay otros VIPs dentro
        cv_vip_entrada.wait(lock, [&] {
            return clientes_dentro < capacidad_maxima;
        });
        vips_esperando--;
        vips_dentro++;
    } else {
        //clientes normales esperan si la tienda está llena o si hay VIPs esperando o dentro
        
        cv_normal_entrada.wait(lock, [&] {
            return clientes_dentro < capacidad_maxima && vips_esperando == 0 && vips_dentro == 0;
        });
    }

    // 2. Entrar en la tienda
    clientes_dentro++;
    std::cout << std::fixed << std::setprecision(1) << get_segundos_transcurridos() << "s: " << cliente_actual.nombre << " entrando" << std::endl;
    lock.unlock(); //sacar el lock para permitir que otros hilos entren en la tienda
    

    // 3. Permanecer dentro tiempoEnTienda segundos
    std::this_thread::sleep_for(std::chrono::duration<double>(cliente_actual.tiempoEnTienda));

    // 4. Comprar numeroTickets entradas
    //aumentar las ganancias totales
    lock.lock();
    ganancias_totales += cliente_actual.numeroTickets * precio_ticket;
    lock.unlock();

    // 5. Salir de la tienda
    lock.lock();
    clientes_dentro--;
    if (cliente_actual.VIP) {
        vips_dentro--;
    }
    std::cout << std::fixed << std::setprecision(1) << get_segundos_transcurridos() << "s: " << cliente_actual.nombre << " saliendo" << std::endl;
    //dejar a los hilos que están esperando para entrar
    // si hay VIPs esperando se les deja pasar primero 
    // si no hay VIPs esperando se deja pasar a los clientes normales
   
    if (vips_esperando > 0 && clientes_dentro < capacidad_maxima) {
        cv_vip_entrada.notify_all(); // Priorizar VIPs
    } else if (vips_esperando == 0 && vips_dentro == 0) { 
        cv_normal_entrada.notify_all(); // Notify clientes normales si no hay VIPs presentes o esperando 
    } else if (vips_esperando == 0 && vips_dentro > 0 && clientes_dentro < capacidad_maxima) {
        cv_vip_entrada.notify_all(); // Notify otros VIPs si hay escapcio disponibles 
    }

    lock.unlock();
}

double simular_tienda(const std::vector<Cliente>& clientes,
                      double precio_ticket,
                      int capacidad_maxima,
                      int n_vips_total) { 
    // resetear las variables globales
    //esto es importante si se llama a la funcion varias veces
    clientes_dentro = 0;
    vips_esperando = 0;
    vips_dentro = 0;
    ganancias_totales = 0.0;

    std::vector<std::thread> threads;
    //crear un vector de indices para ordenar los clientes por su retraso de ingreso
    //esto ayuda a lanzar los hilos de una manera que favorece naturalmente a los VIPs si sus retrasos son más cortos,
    //pero la lógica de prioridad principal está dentro de las variables de condición de cliente_hilo.
    
    std::vector<int> indices(clientes.size());
    std::iota(indices.begin(), indices.end(), 0);

    std::sort(indices.begin(), indices.end(), [&](int a, int b) {
        if (clientes[a].VIP != clientes[b].VIP) {
            return clientes[a].VIP; // True (VIP) va antes de false (non-VIP)
        }
        return clientes[a].retrasoIngreso < clientes[b].retrasoIngreso;
    });

    for (int cliente_idx : indices) {
        threads.emplace_back(cliente_hilo, clientes[cliente_idx], precio_ticket, capacidad_maxima);
    }

    for (auto& th : threads) {
        if (th.joinable()) {
            th.join();
        }
    }

    return ganancias_totales;
}
#include <iostream>
#include <fstream>
#include "json.hpp"
#include <string>
#include "clasesFantasy.hpp"

using namespace std;
using json = nlohmann::json;

int main() {
    ifstream bootstrap("bootstrap-static.json");
    ifstream fixtures("fixtures.json");
    json j;//bootstrap
    json f;//fixtures
    //leer archivo
    bootstrap >> j;
    fixtures >> f;
    //acceder a elements que es donde esta la informacion de los jugadores
    //auto poque no sabemos el tipo de dato
    if (!j.contains("elements") || j["elements"].empty()) {
      cerr << "Error: No hay jugadores en el JSON" << endl;
      return 1;
  }
    auto jugadores = j["elements"];
    auto posiciones = j["element_types"];
          
    Jugador jugador1(jugadores[0]);  // CREACIÓN DEL OBJETO
          
    cout << "El jugador " << jugador1.getNombre() 
     << " " << jugador1.getApellido()
     << " tiene el ID " << jugador1.getId()
     << " y es " << jugador1.identificarPosicion() << endl;
          

   Jugador jugador2(jugadores[4]);

    cout << "el jugador " << jugador2.getNombre() 
            << " " << jugador2.getApellido() << " tiene el id "<<jugador2.getId()
            <<" es " << jugador2.identificarPosicion() << endl;  
    int puntos_partido = jugador2.sumarPuntosPartidos(0);
     int puntos_totales = jugador2.getPuntosTotales();
    cout << "y su puntaje es: " << puntos_partido << endl;
    cout << "y su puntaje total es: " << puntos_totales << endl;
  //    auto equipos = j["teams"];
  // equipo equipo1(equipos[1]);
  // equipo equipo2(equipos[2]);
  // compararEquipos comparar;
  // comparar.comparar(1,2);
  /*int puntos_partido = equipo1.getPuntosPartido(0);
  int puntos_totales= equipo1.getPuntosTotalesEquipo();
    cout << "el equipo " << equipo1.getNombre() << " tiene el id " << equipo1.getId() << endl;
    cout << "y su puntaje es: " << puntos_partido << endl;
    cout << "y su puntaje total es: " << puntos_totales << endl;*/
   /* int id_equipo = equipo1.getId();
    buscarJugadores buscar(id_equipo);
    auto jugadores_equipo = buscar.buscarJugador(id_equipo);
    cout<< "jugadores del equipo " << equipo1.getNombre() << endl;
    for (auto& jugador : jugadores_equipo) {
        cout << "el jugador " << jugador.getNombre() 
            << " " << jugador.getApellido() << " tiene el id "<<jugador.getId()
            <<" es " << jugador.identificarPosicion() << endl;
    }*/
    return 0;
  
}
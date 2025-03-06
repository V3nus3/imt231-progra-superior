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
    auto jugadores = j["elements"];
    auto posiciones = j["element_types"];
    
    Jugador jugador1(jugadores[0]);
    cout << "el jugador" << jugador1.getNombre() 
            << " " << jugador1.getApellido() << " tiene el id "<<jugador1.getId()
            <<" es " << jugador1.identificarPosicion() << endl;
    Jugador jugador2(jugadores[4]);
  int puntos_partido = jugador2.sumarPuntosPartidos();
    cout << "el jugador" << jugador2.getNombre() 
            << " " << jugador2.getApellido() << " tiene el id "<<jugador2.getId()
            <<" es " << jugador2.identificarPosicion() << endl;  
cout << "y su puntaje es: " << puntos_partido << endl;
    //recorrer jugadores
   /* for (auto& jugador : jugadores) {
        if (jugador["id"] == 1) {
            cout << "el jugador" << jugador["first_name"] 
            << " " << jugador["second_name"] << " tiene el id "<<jugador["id"]
            <<" es " << endl;
            int id_posicion = jugador["element_type"];
            for (auto& posicion : posiciones) {
                if (posicion["id"] == id_posicion) {
                    cout << posicion["singular_name"] << endl;
                }
            }
            float valor = jugador["now_cost"];
            cout<< "y su precio es: " << valor/10 << "millones"<<endl;
        }
    }*/
    return 0;
}
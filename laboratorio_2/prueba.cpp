#include <iostream>
#include <fstream>
#include "json.hpp"
#include <string>

using json = nlohmann::json;
using namespace std;

int main() {
    //abrir archivo
    ifstream archivo("bootstrap-static.json");
    json j;
    //leer archivo
    archivo >> j;
    //acceder a elements que es donde esta la informacion de los jugadores
    //auto poque no sabemos el tipo de dato
    auto jugadores = j["elements"];
    auto posiciones = j["element_types"];
    //recorrer jugadores
    for (auto& jugador : jugadores) {
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
    }
    
    return 0;
}
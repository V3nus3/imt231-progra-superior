#include <string>
#include <iostream>
#include "clasesFantasy.hpp"
#include "json.hpp"
#include <fstream>

using namespace std;
using json = nlohmann::json;
//abrir archivo
abrir_archivo::abrir_archivo() {
    ifstream bootstrap("bootstrap-static.json");
    ifstream fixtures("fixtures.json");
   
    //leer archivo
    bootstrap >> j;
    fixtures >> f;
}
abrir_archivo::~abrir_archivo() {
}
/*abrir_archivo::abrir_archivo(const json &archivo) {
   ifstream bootstrap("bootstrap-static.json");
    ifstream fixtures("fixtures.json");
    json j;//bootstrap
    json f;//fixtures
    //leer archivo
    bootstrap >> j;
    fixtures >> f;
    
}*/

//jugador
Jugador::Jugador() {
    nombre = "";
    apellido = "";
    id = 0;
    id_posicion = 0;
    precio = 0.0;
    puntos_partido = 0;
    puntos_totales = 0;
}
Jugador::~Jugador() {
}
Jugador::Jugador(const json &data) {
    nombre = data["first_name"];
    apellido = data["second_name"];
    id = data["id"];
    id_posicion = data["element_type"];
    precio = data["now_cost"];
    puntos_partido = 0;
    puntos_totales = 0;
}
string Jugador::getNombre() {
    return nombre;
}
string Jugador::getApellido() {
    return apellido;
}
int Jugador::getId() {
    return id;
}
int Jugador::getIdPosicion() {
    return id_posicion;
}
int Jugador::getIdEquipo() {
    return equipo_id;
}
float Jugador::getPrecio() {
    return precio/10;
} 
string Jugador::identificarPosicion() {
    abrir_archivo archivo;
    auto posiciones = archivo.j["element_types"];
    for (auto& posicion : posiciones) {
        if (posicion["id"] == id_posicion) {
            return posicion["singular_name"];
        }
    }
    return "";
}
int Jugador::sumarPuntosPartidos() {
    
    if (id_posicion == 1) {
        goalkeeper portero(id);
        puntos_partido = portero.sumarPuntos(id);
    }
    /*else if (id_posicion == 2) {
        /* code 
    }
    else if (id_posicion == 3) {
        /* code 
    }
    else if (id_posicion == 4)
    {
        /* code 
    }*/
    
    
    
    return puntos_partido;
}
//equipo
equipo::equipo() {
    name = "";
    short_name = "";
    id = 0;
    puntos_partido = 0;
    puntos_totales = 0;
}
equipo::~equipo() {
}
equipo::equipo(const json &dataEquipo) {
    name = dataEquipo["name"];
    short_name = dataEquipo["short_name"];
    id = dataEquipo["id"];
    puntos_partido = 0;
    puntos_totales = 0;
}
string equipo::getNombre() {
    return name;
}
string equipo::getShortName() {
    return short_name;
}
int equipo::getId() {
    return id;
}
void equipo::addJugador(Jugador jugador) {
    jugadores.push_back(jugador);
}
// determinar si es local o visitante
esLocal::esLocal() {
    local = false;
    esVisitante = false;
   
}
esLocal::~esLocal() {}
bool esLocal::determinarLocal(int idJugador) {
    int id=idJugador;
    abrir_archivo archivo;
    auto partidos = archivo.f[0];
    auto stats = partidos["stats"];
    int local = partidos["team_h"];
    int visitante = partidos["team_a"];
    for (auto& stat : stats) {
        //revisas h
        for(auto& eventoH : stat["h"]){
            if(eventoH["element"] == id){
                local = true;
                break;
            }
        }
        //revisas a
        for(auto& eventoA : stat["a"]){
            if(eventoA["element"] == id){
                esVisitante = true;
                break;
            }
        }
    }
    if (local) {
        return true;
    }
    else {
        return false;
    } 
}
//goalkeeper

goalkeeper::goalkeeper() {
    id = 0;
    id_posicion = 1;
    puntos_patido = 0;
    puntos_totales = 0;
}
goalkeeper::goalkeeper(int id) {
    this->id = id;
    id_posicion = 1;
    puntos_patido = 0;
    puntos_totales = 0;
}
goalkeeper::~goalkeeper() {
}
// Reglas de puntuación 
// - Portería a cero: +4
// - Cada 3 atajadas: +1
// - Penalti atajado: +5
// - Goles encajados: -1 punto cada 2 goles
// - Gol marcado: +10
// - Tarjeta amarilla: -1
// - Tarjeta roja: -3
// - Minutos jugados: 1-59 => +1, 60+ => +2
int goalkeeper::sumarPuntos(int id) {
    abrir_archivo archivo;
    auto partidos = archivo.f[0];
    auto stats = partidos["stats"];
    int puntos = 0;
    int goles_local = partidos["team_h_score"];
    int goles_visitante = partidos["team_a_score"];
    
    esLocal eslocal; 
    bool es_local = eslocal.determinarLocal(id);
  
    int goles_encajados = 0;
    if(es_local){
        goles_encajados = goles_visitante;//si es local los goles encajados son los del visitante
    }else if(!es_local){
        goles_encajados = goles_local;
    }
    //-1 punto cada 2 goles
    if(goles_encajados >= 2){
        puntos -= goles_encajados/2;
    }
    //porteria 0
    if(goles_encajados == 0){
        puntos += 4;
    }
    for (auto& stat : stats) {
        string identifier = stat["identifier"];
        // Seleccionamos el array a procesar según esLocal
        auto eventos = es_local ? stat["h"] : stat["a"];
        for (auto& evento : eventos) {
            if (evento["element"] == id) {
                int valor = evento["value"];
                if (identifier == "goals_scored") {
                    puntos += 10 * valor;
                }
                else if (identifier == "penalties_saved") {
                    puntos += 5 * valor;
                }
                else if (identifier == "saves") {
                    puntos += (valor / 3);
                }
                else if (identifier == "yellow_cards") {
                    puntos -= valor;
                }
                else if (identifier == "red_cards") {
                    puntos -= 3 * valor;
                }
            
            }
        }
    }
    return puntos;
}
int goalkeeper::getPuntosTotales() {
    return puntos_totales;
}

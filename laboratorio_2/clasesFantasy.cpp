#include <string>
#include <iostream>
#include "clasesFantasy.hpp"
#include "json.hpp"
#include <fstream>
#include <vector>
#include <unordered_map>

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
//VirtualJugador
VirtualJugador :: VirtualJugador(){
    nombre = "desconocido";
    apellido = "desconocido";
    id = 0;
    id_posicion = 0;
    precio = 0.0;
    puntos_partido = 0;
    puntos_totales = 0;
}
VirtualJugador::VirtualJugador(const json &data) {
    nombre = data.value("first_name", "desconocido");
    apellido = data.value("second_name", "desconocido");
    id= data.value("id", 0);
    id_posicion = data.value("element_type", 0);
    equipo_id = data.value("team", 0);
    precio = data.value("now_cost", 0);

    
}
VirtualJugador::~VirtualJugador() {
}
string VirtualJugador::getNombre() {return nombre;}
string VirtualJugador::getApellido() {return apellido;}
int VirtualJugador::getId() { return id;}
int VirtualJugador::getIdPosicion() { return id_posicion;}
int VirtualJugador::getIdEquipo() { return equipo_id;}
float VirtualJugador::getPrecio() {return precio/10;}
//jugador

Jugador::Jugador() : VirtualJugador() {
    nombre = "desconocido";
    apellido = "desconocido";
    id = -1;
    id_posicion = -1;
    equipo_id = -1;
    precio = 0.0;
    puntos_partido = 0;
    puntos_totales = 0; 
}

Jugador::Jugador(const json &data) : VirtualJugador(data) {
   
   /* cout << "Jugador creado -> Nombre: [" << getNombre() 
    << "], Apellido: [" << getApellido() 
    << "], ID: [" << getId() 
    << "], Posición: [" << getIdPosicion() 
    << "], Precio: [" << getPrecio() << "]" << endl;*/
    puntos_partido = 0;
    puntos_totales = 0;
}

string Jugador::identificarPosicion()const {
    static unordered_map<int, string> posiciones;
    
    if (posiciones.empty()) {
        abrir_archivo archivo;//asi solo se abre una vez y es mas rapido
     auto posicionesJson = archivo.j["element_types"];
        for (auto& posicion : posicionesJson) {
            posiciones[posicion["id"]] = posicion.value("singular_name", "desconocido");
        }
    }
    auto it = posiciones.find(id_posicion);
    if (it != posiciones.end()) {
        return it->second;
    }else{
        return "desconocido";
    }
    
}
Jugador::~Jugador() {}

int Jugador::sumarPuntosPartidos(int i) {
    
    if (id_posicion == 1) {
        goalkeeper portero(id);
        puntos_partido = portero.sumarPuntos(id,i);
    }
    else if (id_posicion == 2) {
        defender defensor;
        puntos_partido = defensor.sumarPuntos(id,i);
    }
    else if (id_posicion == 3) {
        midfielder mediocampista;
        puntos_partido = mediocampista.sumarPuntos(id,i);
    }
    else if (id_posicion == 4) {
        Forward delantero;
        puntos_partido = delantero.sumarPuntos(id,i);
    }
    return puntos_partido;
}
// int Jugador::calcularPuntos(const json &fixture, bool esLocal) const{
//     int puntos =0;
//     auto stats = fixture["stats"];
//     int goles_local = fixture["team_h_score"];
//     int goles_visitante = fixture["team_a_score"];
//     int goles_encajados = 0;

//     if(esLocal){
//         goles_encajados = goles_visitante;//si es local los goles encajados son los del visitante
//     }else if(!esLocal){
//         goles_encajados = goles_local;
//     }
//     //-1 punto cada 2 goles
//     if(goles_encajados >= 2){
//         puntos -= goles_encajados/2;
//     }
//     //porteria 0
//     if(goles_encajados == 0){
//         puntos += 4;
//     }
//     for (auto& stat : stats) {
//         string identifier = stat["identifier"];
//         // Seleccionamos el array a procesar según esLocal
//         auto eventos = esLocal ? stat["h"] : stat["a"];
//         for (auto& evento : eventos) {
//             if (evento["element"] == id) {
//                 int valor = evento["value"];
//                 if (identifier == "goals_scored") {
//                     puntos += 10 * valor;
//                 }
//                 else if (identifier == "penalties_saved") {
//                     puntos += 5 * valor;
//                 }
//                 else if (identifier == "saves") {
//                     puntos += (valor / 3);
//                 }
//                 else if (identifier == "yellow_cards") {
//                     puntos -= valor;
//                 }
//                 else if (identifier == "red_cards") {
//                     puntos -= 3 * valor;
//                 }
            
//             }
//         }
//     }
//     return puntos;
// }
int Jugador::getPuntosTotales() {
    abrir_archivo archivo;
    int total=0;
    //int size = archivo.f.size();
    //se debe limitar el tamaño del for para que no se demore tanto
    //hay algunos partidos donde no se encuentran los stats de los jugadores
    int size = 50;
    for(int i=0; i<size; i++){
        total += sumarPuntosPartidos(i );
    }
    return total;
}
// int Jugador::getPuntosTotales( ) {
//     abrir_archivo archivo;
//     int total=0;
//     int limite= min((int)archivo.f.size(),50);
//     esLocal eslocal; 
//     bool es_local;
//     for(int i=0; i <50; i++){
//         es_local = eslocal.determinarLocal(id, i);
//         total += calcularPuntos(archivo.f[i], es_local);
//     }
//     puntos_totales = total;
//     return puntos_totales;
// }
   
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

int equipo::getPuntosPartido(int i) {
    abrir_archivo archivo;
    auto partidos = archivo.f[i];
    int goles_local = partidos["team_h_score"];
    int goles_visitante = partidos["team_a_score"];
    int total = 0;
    esLocal eslocal; 
    bool es_local = eslocal.determinarLocal(id, i);
    if(es_local){
        total= goles_local;
    }
    else if(!es_local){
        total = goles_visitante;
    }

    puntos_partido = total;
    return puntos_partido;
}
int equipo::getPuntosTotalesEquipo() {
    int total = 0;
    for(int i=0; i<50; i++){
        total += getPuntosPartido(i);
    }
    puntos_totales = total;
    return puntos_totales;
}
//buscarJugadores
buscarJugadores::buscarJugadores(int id) {
    this->id = id;

}
buscarJugadores::~buscarJugadores() {
}
/*vector<Jugador> buscarJugadores::buscarJugador(int id) {
    abrir_archivo archivo;
    auto jugadores = archivo.j["elements"];
    vector<Jugador> jugadores_equipo;
    for (auto& jugador : jugadores) {
        if (jugador["team"] == id) {
            Jugador jugador1(jugador);
            jugadores_equipo.push_back(jugador1);
        }
    }
    return jugadores_equipo;
}*/
//compararEquipos
compararEquipos::compararEquipos() {
    equipo1 = equipo();
    equipo2 = equipo();
}
compararEquipos::~compararEquipos() {
}
void compararEquipos::comparar(int id1, int id2) {
    abrir_archivo archivo;
    auto equipos = archivo.j["teams"];
    equipo1 = equipo(equipos[id1]);
    equipo2 = equipo(equipos[id2]);
    int puntos_equipo1 = equipo1.getPuntosTotalesEquipo();
    int puntos_equipo2 = equipo2.getPuntosTotalesEquipo();
    if (puntos_equipo1 > puntos_equipo2) {
        cout << "El equipo " << equipo1.getNombre() << " tiene más puntos que el equipo " << equipo2.getNombre() << endl;
    }
    else if (puntos_equipo1 < puntos_equipo2) {
        cout << "El equipo " << equipo2.getNombre() << " tiene más puntos que el equipo " << equipo1.getNombre() << endl;
    }
    else {
        cout << "Los equipos " << equipo1.getNombre() << " y " << equipo2.getNombre() << " tienen la misma cantidad de puntos" << endl;
    }
}
// determinar si es local o visitante
esLocal::esLocal() {
    local = false;
    esVisitante = false;
   
}
esLocal::~esLocal() {}
bool esLocal::determinarLocal(int idJugador, int i) {
    int id=idJugador;
    abrir_archivo archivo;
    auto partidos = archivo.f[i];
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
int goalkeeper::sumarPuntos(int id, int i) {
    abrir_archivo archivo;
    auto partidos = archivo.f[i];
    auto stats = partidos["stats"];
    int puntos = 0;
    int goles_local = partidos["team_h_score"];
    int goles_visitante = partidos["team_a_score"];
    
    esLocal eslocal; 
    bool es_local = eslocal.determinarLocal(id, i);
  
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
int goalkeeper::calcularPuntos(const json &fixture, bool esLocal) const{
    int puntos =0;
    auto stats = fixture["stats"];
    int goles_local = fixture["team_h_score"];
    int goles_visitante = fixture["team_a_score"];
    int goles_encajados = 0;

    if(esLocal){
        goles_encajados = goles_visitante;//si es local los goles encajados son los del visitante
    }else if(!esLocal){
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
        auto eventos = esLocal ? stat["h"] : stat["a"];
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
/*int goalkeeper::getPuntosTotales(int id) {
    abrir_archivo archivo;
    int total=0;
    for(int i=0; i==archivo.f.size(); i++){
        total += sumarPuntos(id,i );
    }
    puntos_totales = total;
    return puntos_totales;
}*/
//defender
defender::defender() {
    id_posicion = 2;
    puntos_patido = 0;
    puntos_totales = 0;
}
defender::~defender() {
}
/*Defensores:
Portería a cero: +4 puntos.
Gol marcado: +6 puntos.
Asistencia: +3 puntos.
Gol en propia puerta: -2 puntos.
Tarjeta amarilla: -1 punto.
Tarjeta roja: -3 puntos.
*/
int defender::sumarPuntos(int id,int i) {
    abrir_archivo archivo;
    auto partidos = archivo.f[i];
    auto stats = partidos["stats"];
    int puntos = 0;
    int goles_local = partidos["team_h_score"];
    int goles_visitante = partidos["team_a_score"];
    esLocal eslocal; 
    bool es_local = eslocal.determinarLocal(id,i);
  
    int goles_encajados = 0;
    if(es_local){
        goles_encajados = goles_visitante;//si es local los goles encajados son los del visitante
    }else if(!es_local){
        goles_encajados = goles_local;
    }
    if(goles_encajados == 0){
        puntos += 4;
    }
    for (auto& stat : stats) {
        string identifier = stat["identifier"];
        auto eventos = es_local ? stat["h"] : stat["a"];
        for (auto& evento : eventos) {
            if (evento["element"] == id) {
                int valor = evento["value"];
                if (identifier == "goals_scored") {
                    puntos += 6 * valor;
                }
                else if (identifier == "assists") {
                    puntos += 3 * valor;
                }
                else if (identifier == "own_goals") {
                    puntos -= 2 * valor;
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
    puntos_patido = puntos;
    return puntos_patido;
}

//midfielder
midfielder::midfielder() {
    id_posicion = 3;
    puntos_patido = 0;
    puntos_totales = 0;
}
midfielder::~midfielder() {
}
/*Mediocampistas:
Gol marcado: +5 puntos.
Asistencia: +3 puntos.
Portería a cero: +1 punto.
Tarjeta amarilla: -1 punto.
Tarjeta roja: -3 puntos.
*/
int midfielder::sumarPuntos(int id, int i) {
    abrir_archivo archivo;
    auto partidos = archivo.f[i];
    auto stats = partidos["stats"];
    int puntos = 0;
    esLocal eslocal; 
    bool es_local = eslocal.determinarLocal(id,i);
  
    for (auto& stat : stats) {
        string identifier = stat["identifier"];
        auto eventos = es_local ? stat["h"] : stat["a"];
        for (auto& evento : eventos) {
            if (evento["element"] == id) {
                int valor = evento["value"];
                if (identifier == "goals_scored") {
                    puntos += 5 * valor;
                }
                else if (identifier == "assists") {
                    puntos += 3 * valor;
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
    puntos_patido = puntos;
    return puntos_patido;
}

//forward
Forward::Forward() {
    id_posicion = 4;
    puntos_patido = 0;
    puntos_totales = 0;
}
Forward::~Forward() {
}
/*Delanteros:
Gol marcado: +4 puntos.
Asistencia: +3 puntos.
Tarjeta amarilla: -1 punto.
Tarjeta roja: -3 puntos.
*/
int Forward::sumarPuntos(int id, int i) {
    abrir_archivo archivo;
    auto partidos = archivo.f[i];
    auto stats = partidos["stats"];
    int puntos = 0;
    esLocal eslocal; 
    bool es_local = eslocal.determinarLocal(id,i);
  
    for (auto& stat : stats) {
        string identifier = stat["identifier"];
        auto eventos = es_local ? stat["h"] : stat["a"];
        for (auto& evento : eventos) {
            if (evento["element"] == id) {
                int valor = evento["value"];
                if (identifier == "goals_scored") {
                    puntos += 4 * valor;
                }
                else if (identifier == "assists") {
                    puntos += 3 * valor;
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
    puntos_patido = puntos;
    return puntos_patido;
}


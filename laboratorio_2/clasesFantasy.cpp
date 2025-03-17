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
// determinar si es local o visitante
esLocal::esLocal() {    local = false;    esVisitante = false;}
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
   
    puntos_partido = 0;
    puntos_totales = 0;
}

Jugador::~Jugador() {}
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

int Jugador::sumarPuntosPartidos(int i) {
    static const unordered_map<int, function<int(int, int)>> posiciones = {
        {1, [](int id, int i) { goalkeeper portero(id); return portero.sumarPuntos(id, i); }},
        {2, [](int id, int i) { defender defensor; return defensor.sumarPuntos(id, i); }},
        {3, [](int id, int i) { midfielder mediocampista; return mediocampista.sumarPuntos(id, i); }},
        {4, [](int id, int i) { Forward delantero; return delantero.sumarPuntos(id, i); }}
    };
    auto it = posiciones.find(id_posicion);
    if (it != posiciones.end()) {//si la posicion es valida
        puntos_partido = it->second(id, i);
    } else {
        puntos_partido = 0;  // Si la posición no es válida, asignamos 0 puntos.
    }

    return puntos_partido;
}

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
}
goalkeeper::~goalkeeper() {}

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
  
    int goles_encajados = es_local ? goles_visitante : goles_local;
    
    //-1 punto cada 2 goles
    if(goles_encajados >= 2){ puntos -= goles_encajados/2;}
    //porteria 0
    if(goles_encajados == 0){  puntos += 4;}
    static const unordered_map<string, int> reglas = {
        {"goals_scored", 10},        // Gol anotado
        {"penalties_saved", 5},      // Penal atajado
        {"yellow_cards", -1},        // Tarjeta amarilla
        {"red_cards", -3}            // Tarjeta roja
    };
    for (const auto& stat : stats) {
        string identifier = stat["identifier"];
        // Seleccionamos el array a procesar según esLocal
        auto eventos = es_local ? stat["h"] : stat["a"];
           
        for (const auto& evento : eventos) {
            if (evento["element"] == id && evento.contains("value")) {
                int valor = evento["value"];

                if (reglas.count(identifier)) {
                    puntos += reglas.at(identifier) * valor;
                } else if (identifier == "saves") { // Regla especial para atajadas
                    puntos += (valor / 3);
                }
          }
        }
    }
    return puntos;
}

//defender
defender::defender() {
    id_posicion = 2;
    puntos_patido = 0;
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
    goles_encajados = es_local ? goles_visitante : goles_local;
    //porteria 0
    if(goles_encajados == 0){  puntos += 4;}
    puntos_patido = puntos;
    static const unordered_map<string, int> reglas = {
        {"goals_scored", 6},        // Gol anotado
        {"assists", 3},             // Asistencia
        {"own_goals", -2},          // Gol en propia puerta
        {"yellow_cards", -1},       // Tarjeta amarilla
        {"red_cards", -3}           // Tarjeta roja
    };
    for (const auto& stat : stats) {
        string identifier = stat["identifier"];
        // Seleccionamos el array a procesar según esLocal
        auto eventos = es_local ? stat["h"] : stat["a"];
           
        for (const auto& evento : eventos) {
            if (evento["element"] == id && evento.contains("value")) {
                int valor = evento["value"];

                if (reglas.count(identifier)) {
                    puntos += reglas.at(identifier) * valor;
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
    
    static const unordered_map<string, int> reglas = {
        {"goals_scored", 5},        // Gol anotado
        {"assists", 3},             // Asistencia
        {"clean_sheets", 1},        // Portería a cero
        {"yellow_cards", -1},       // Tarjeta amarilla
        {"red_cards", -3}           // Tarjeta roja
    };
    for (const auto& stat : stats) {
        string identifier = stat["identifier"];
        // Seleccionamos el array a procesar según esLocal
        auto eventos = es_local ? stat["h"] : stat["a"];
           
        for (const auto& evento : eventos) {
            if (evento["element"] == id && evento.contains("value")) {
                int valor = evento["value"];

                if (reglas.count(identifier)) {
                    puntos += reglas.at(identifier) * valor;
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
  
    static const unordered_map<string, int> reglas = {
        {"goals_scored", 4},        // Gol anotado
        {"assists", 3},             // Asistencia
        {"yellow_cards", -1},       // Tarjeta amarilla
        {"red_cards", -3}           // Tarjeta roja
    };
    for (const auto& stat : stats) {
        string identifier = stat["identifier"];
        // Seleccionamos el array a procesar según esLocal
        auto eventos = es_local ? stat["h"] : stat["a"];
           
        for (const auto& evento : eventos) {
            if (evento["element"] == id && evento.contains("value")) {
                int valor = evento["value"];

                if (reglas.count(identifier)) {
                    puntos += reglas.at(identifier) * valor;
                } 
          }
        }
    }
    puntos_patido = puntos;
    return puntos_patido;
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
string equipo::getNombre() {   return name;}
string equipo::getShortName() {   return short_name;}
int equipo::getId() {    return id;}

int equipo::getPuntosPartido(int i) {
    abrir_archivo archivo;
    auto partidos = archivo.f[i];
    int goles_local = partidos["team_h_score"];
    int goles_visitante = partidos["team_a_score"];
    int total = 0;
    esLocal eslocal; 
    bool es_local = eslocal.determinarLocal(id, i);
     total = es_local ? goles_visitante : goles_local;

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
buscarJugadores::buscarJugadores(int id) {   this->id = id;}
buscarJugadores::~buscarJugadores() {}
vector<Jugador> buscarJugadores::buscarJugador(int id) {
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
}
//compararEquipos
compararEquipos::compararEquipos() {    equipo1 = equipo();   equipo2 = equipo();}
compararEquipos::~compararEquipos() {}
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



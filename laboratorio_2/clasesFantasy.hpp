#ifndef CLASESFANTASY_HPP
#define CLASESFANTASY_HPP

#include <string>
#include <iostream>
#include "json.hpp"
#include <fstream>
#include <vector>

using namespace std;
using json = nlohmann::json;
class abrir_archivo{
    public:
        abrir_archivo();
        ~abrir_archivo();
        abrir_archivo(const json &archivo);
        json obtenerJson();

        json f;
        json j;
};
class Jugador {
    public:
        Jugador();//constructor por defecto
        ~Jugador();//destructor
        Jugador( const json &data);//
        string getNombre();
        string getApellido();
        int getId();
        int getIdPosicion();
        int getIdEquipo();
        float getPrecio();
        string identificarPosicion();
       
        int sumarPuntosPartidos();
    private:
        string nombre;
        string apellido;
        int equipo_id;
        int id;
        int id_posicion;
        float precio;
        int puntos_partido;
        int puntos_totales;
};

class equipo{
    public:
        equipo();
        ~equipo();
        equipo(const json &dataEquipo);
        string getNombre();
        string getShortName();
        int getId();
        void addJugador(Jugador jugador);
    private:
        string name;
        string short_name;
        int id;
        vector<Jugador> jugadores;
        int puntos_partido;
        int puntos_totales;
};
/*class mostrar_puntos{
    public:
        mostrar_puntos();
        mostrar_puntos(string nombre, int puntos);
        string obtenerNombre();
        int obtenerPuntos();
        void establecerNombre(string nombre);
        void establecerPuntos(int puntos);
    private:
        string nombre;
        int puntos;
};
*/
class esLocal{
    public:
        esLocal();
        ~esLocal();
        bool determinarLocal(int idJugador);
    private:
        bool local;
       bool esVisitante;
};
class goalkeeper : public Jugador{
    public:
        goalkeeper() ;
        goalkeeper(int id);
        ~goalkeeper();
       // goalkeeper(const json &data);
        int sumarPuntos(int id);
        int getPuntosTotales();
    private:
       int id;
        int id_posicion;
        int puntos_patido;
        int puntos_totales;
};
class defender : public Jugador{
    public:
        defender();
        ~defender();
        defender(string nombre, int id, int id_posicion, float precio, int puntos);
        int getId();
        int getIdPosicion();
        int sumarPuntos(int puntos);
        int getPuntosTotales();
    private:
 
        int id_posicion=2;
        int puntos_patido=0;
        int puntos_totales=0;
};
class midfielder : public Jugador{
    public:
        midfielder();
        ~midfielder();
        midfielder(string nombre, int id, int id_posicion, float precio, int puntos);
        int getId();
        int getIdPosicion();
        int sumarPuntos(int puntos);
        int getPuntosTotales();
    private:
 
        int id_posicion=3;
        int puntos_patido=0;
        int puntos_totales=0;
};
class forward : public Jugador{
    public:
        forward();
        ~forward();
        forward(string nombre, int id, int id_posicion, float precio, int puntos);
        int getId();
        int getIdPosicion();
        int sumarPuntos(int puntos);
        int getPuntosTotales();
    private:

        int id_posicion=4;
        int puntos_patido=0;
        int puntos_totales=0;
};

#endif // CLASESFANTASY_HPP
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
class esLocal{
    public:
        esLocal();
        ~esLocal();
        bool determinarLocal(int idJugador, int i);
    private:
        bool local;
       bool esVisitante;
};
class VirtualJugador {
    public:
        VirtualJugador();
        VirtualJugador( const json &data);
        ~VirtualJugador(); 
        string getNombre();
         string getApellido();
        int getId();
        int getIdPosicion();
        int getIdEquipo();
        float getPrecio();
    protected:
        string nombre;
        string apellido;
        int equipo_id;
        int id;
        int id_posicion;
        float precio;
        int puntos_partido;
        int puntos_totales;
};
class Jugador : public VirtualJugador {
    public:
        Jugador();
        ~Jugador();
        Jugador(const json& jugador);
        string identificarPosicion()const ;
          
       int sumarPuntosPartidos(int i);
        int getPuntosTotales();

    protected:
        
        int puntos_partido;
        int puntos_totales;
    };
    class goalkeeper : public Jugador{
        public:
            goalkeeper() ;
            goalkeeper(int id);
            virtual ~goalkeeper();
            int sumarPuntos(int id, int i);
            
        private:
           int id;
            int id_posicion;
            int puntos_patido;
        
    };
    class defender : public Jugador{
        public:
            defender();
            virtual ~defender();
            int sumarPuntos(int id, int i);
            
        private:
     
            int id_posicion;
            int puntos_patido;
           
    };
    class midfielder : public Jugador{
        public:
            midfielder();
            virtual ~midfielder();
            int sumarPuntos(int id, int i);
           
        private:
     
            int id_posicion;
            int puntos_patido;
           
    };
    class Forward : public Jugador{
        public:
            Forward();
            int sumarPuntos(int id, int i);
            virtual ~Forward();
        private:
    
            int id_posicion;
            int puntos_patido;
       
    };
class equipo{
    public:
        equipo();
        ~equipo();
        equipo(const json &dataEquipo);
        string getNombre();
        string getShortName();
        int getId();
        int getPuntosPartido(int i);
        int getPuntosTotalesEquipo();

    private:
        string name;
        string short_name;
        int id;
        int puntos_partido;
        int puntos_totales;
};
class buscarJugadores : public equipo{ 
    public:
        buscarJugadores(int id);
        ~buscarJugadores();
        vector<Jugador> buscarJugador(int id);
    private:
        int id;
};
//comparar equipos
class compararEquipos : public equipo{
    public:
        compararEquipos();
        ~compararEquipos();
        void comparar(int id1, int id2);
    private:
        equipo equipo1;
        equipo equipo2;
};

#endif // CLASESFANTASY_HPP
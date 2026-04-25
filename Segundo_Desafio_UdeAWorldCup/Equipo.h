// Equipo.h
// Representa una seleccion del Mundial.
// Une casi todas las clases anteriores:
//   - Federacion (por valor)
//   - ConfederacionContinental (por valor)
//   - DirectorTecnico* (memoria dinamica)
//   - Lista<Jugador*> (cada jugador en memoria dinamica)
//   - EstadisticasEquipo (por valor)

#ifndef EQUIPO_H
#define EQUIPO_H

#include <iostream>
#include <string>

#include "tipos.h"
#include "Lista.h"
#include "Federacion.h"
#include "ConfederacionContinental.h"
#include "DirectorTecnico.h"
#include "Jugador.h"
#include "EstadisticasEquipo.h"

using namespace std;

class Equipo {
public:
    string nombre;
    string codigoFIFA;          // ej: "ARG", "COL", "BRA"
    int rankingFIFA;            // posicion en el ranking FIFA
    int titulosMundiales;       // mundiales ganados (historico)

    // Datos historicos para la formula de Poisson:
    //   λ = mu * (GFA/mu)^alpha * (GCB/mu)^beta
    // Aqui guardamos GFA (promedio de goles a favor por partido)
    // y GCB (promedio de goles en contra por partido).
    double promedioGolesFavor;
    double promedioGolesContra;

    // Composicion fuerte (siempre existen, viven con el equipo):
    Federacion federacion;
    ConfederacionContinental confederacion;
    EstadisticasEquipo estadisticas;

    // Memoria dinamica:
    DirectorTecnico* dt;        // un solo DT por equipo
    Lista<Jugador*> jugadores;  // 26 jugadores convocados al Mundial

    // Constructores
    Equipo();
    Equipo(string _nombre, string _codigoFIFA, int _rankingFIFA);
    Equipo(const Equipo& otro);   // copia profunda
    ~Equipo();

    // Setters basicos
    void setFederacion(const Federacion& f);
    void setConfederacion(const ConfederacionContinental& c);
    void setDirectorTecnico(DirectorTecnico* nuevoDT);
    void setPromedios(double gfa, double gcb);
    void setTitulosMundiales(int t);

    // Agregar un jugador a la lista (toma posesion del puntero)
    void agregarJugador(Jugador* j);

    // Devuelve el jugador en la posicion indicada (sin copiar)
    Jugador* getJugador(int indice) const;
    int getCantidadJugadores() const;

    // Devuelve cuantos jugadores estan ACTIVOS (no expulsados)
    int contarJugadoresActivos() const;

    // Reactiva a todos los jugadores que estaban inactivos.
    // Se llama al inicio de cada nuevo partido (la roja sancionaba el
    // partido siguiente, no todo el torneo).
    void reactivarJugadores();

    // Registra el resultado de un partido en las estadisticas del equipo.
    void registrarResultado(int golesProp, int golesRiv);

    // Getters generales
    string getNombre() const;
    string getCodigoFIFA() const;
    int getRanking() const;
    double getPromedioGF() const;
    double getPromedioGC() const;
    EstadisticasEquipo& getEstadisticas();        // referencia (modificable)
    const EstadisticasEquipo& getEstadisticas() const; // version const

    // Imprime los jugadores convocados (los primeros 11 actuales)
    void mostrarPlantilla() const;

    // Dos equipos son iguales si tienen el mismo codigo FIFA
    bool operator==(const Equipo& otro) const;

    // Para ordenar equipos: primero los que tienen mas puntos.
    bool operator<(const Equipo& otro) const;

    // Imprime "ARG Argentina (CONMEBOL) - Pts=...".
    friend ostream& operator<<(ostream& os, const Equipo& e);
};

#endif // EQUIPO_H

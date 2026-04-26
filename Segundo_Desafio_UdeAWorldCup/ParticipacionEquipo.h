// ParticipacionEquipo.h
// Representa COMO PARTICIPA un equipo en UN partido especifico.
// Sirve de puente entre Equipo y Partido.
//
// Idea importante:
//   - Equipo guarda los 26 jugadores de la convocatoria oficial.
//   - Cada partido escoge 11 (los "convocados al partido", o sea la alineacion).
//   - Por cada convocado, llevamos un EstadisticasJugadorPartido (snapshot).
//
// Punteros:
//   - equipo y los Jugador* convocados son SOLO REFERENCIAS (no se borran aqui).
//     El dueno real es la clase Equipo.
//   - Las EstadisticasJugadorPartido SI son creadas con 'new' aca, asi que
//     hay que liberarlas en el destructor.
//
// Forward declaration de Partido para romper la dependencia circular:
//   Partido tiene ParticipacionEquipo* y ParticipacionEquipo guarda Partido*.

#ifndef PARTICIPACIONEQUIPO_H
#define PARTICIPACIONEQUIPO_H

#include <iostream>
#include <string>

#include "tipos.h"
#include "Lista.h"
#include "Equipo.h"
#include "Jugador.h"
#include "EstadisticasJugadorPartido.h"

using namespace std;

// Forward declaration: solo le decimos al compilador "esta clase existe,
// despues se implementa". No necesitamos su definicion completa porque
// solo guardamos un puntero a ella.
class Partido;

class ParticipacionEquipo {
public:
    Equipo* equipo;                    // referencia (no se borra aqui)
    Partido* partido;                  // referencia al partido al que pertenece
    bool esLocal;                      // true = local, false = visitante
    int golesAnotados;                 // goles del equipo en este partido

    // Los 11 convocados al partido (subconjunto de equipo->jugadores)
    // Son SOLO punteros prestados, no los borramos en el destructor.
    Lista<Jugador*> convocados;

    // Una EstadisticasJugadorPartido por cada convocado.
    // Estas SI se crean con 'new' y se liberan en el destructor.
    Lista<EstadisticasJugadorPartido*> estadisticasJugadores;

    ParticipacionEquipo();
    ParticipacionEquipo(Equipo* eq, bool _esLocal);
    ParticipacionEquipo(const ParticipacionEquipo& otro);  // copia profunda de las stats
    ~ParticipacionEquipo();

    // Toma los primeros 11 jugadores ACTIVOS del equipo y los convoca.
    // Por cada uno crea una EstadisticasJugadorPartido en memoria dinamica.
    void convocarTitulares();

    // Permite agregar un jugador especifico (por ej. para suplencias)
    void convocarJugador(Jugador* j);

    // Suma un gol al equipo en este partido y lo asigna al jugador en el indice
    // dado. Si el indice no es valido, solo suma al equipo.
    void registrarGolDeJugador(int indice);

    // Registra eventos sobre el jugador en el indice dado.
    void registrarTarjetaDeJugador(int indice, bool esRoja);
    void registrarFaltaDeJugador(int indice);

    // Setters / getters basicos
    void setEsLocal(bool b);
    void setPartido(Partido* p);
    void setGolesAnotados(int g);

    Equipo* getEquipo() const;
    Partido* getPartido() const;
    bool getEsLocal() const;
    int getGolesAnotados() const;
    int getCantidadConvocados() const;
    Jugador* getConvocado(int indice) const;
    EstadisticasJugadorPartido* getEstadisticasJugador(int indice) const;

    // Cuando termina el partido: vuelca las estadisticas individuales
    // sobre EstadisticasJugador de cada jugador, y aumenta su PJ.
    void volcarEstadisticasAcumuladas();

    // Operador == : misma participacion si es el mismo equipo y mismo lado
    bool operator==(const ParticipacionEquipo& otro) const;

    friend ostream& operator<<(ostream& os, const ParticipacionEquipo& p);

private:
    // Borra todas las EstadisticasJugadorPartido y vacia la lista.
    // Lo usamos en el destructor y en el operator= si lo agregamos despues.
    void liberarEstadisticas();
};

#endif // PARTICIPACIONEQUIPO_H

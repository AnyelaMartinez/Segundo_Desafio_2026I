// EstadisticasJugador.h
// Estadisticas historicas (acumuladas) de un jugador.
// Incluye datos que vienen de torneos anteriores + los del mundial actual.
// El atributo golesEnMundial solo cuenta los goles del torneo en curso,
// que sirve para sacar el ranking de goleadores al final.

#ifndef ESTADISTICASJUGADOR_H
#define ESTADISTICASJUGADOR_H

#include <string>
#include <iostream>
using namespace std;

class EstadisticasJugador {
public:
    int partidosJugados;
    int goles;              // goles totales (historico + mundial)
    int golesEnMundial;     // solo los del mundial actual
    int minutosJugados;
    int asistencias;        // siempre 0 en v1.0 (asi lo pide el desafio)
    int faltas;
    int tarjetasAmarillas;
    int tarjetasRojas;

    EstadisticasJugador();
    EstadisticasJugador(const EstadisticasJugador& otro); // constructor de copia

    // Actualiza todos los contadores sumando lo que paso en UN partido.
    void actualizarEstadisticas(int goles, int minutos, int amarillas,
                                int rojas, int faltas);

    // Metodos granulares (uno por tipo de evento).
    // Son mas comodos cuando la clase Jugador registra eventos uno a uno.
    void actualizarMinutos(int min);
    void actualizarGol(bool esMundial);
    void actualizarTarjetaAmarilla();
    void actualizarTarjetaRoja();
    void actualizarFalta();
    void aumentarPartidosJugados();

    // Deja todos los contadores en cero.
    void reiniciar();

    // Getters simples
    int getGoles() const;
    int getGolesEnMundial() const;
    int getMinutos() const;
    int getTarjetasAmarillas() const;
    int getTarjetasRojas() const;
    int getFaltas() const;
    int getPartidosJugados() const;

    // Devuelve un texto corto con el resumen de las estadisticas.
    string obtenerResumen() const;

    // Funcion amiga: permite cout << estadisticas;
    friend ostream& operator<<(ostream& os, const EstadisticasJugador& e);
};

#endif // ESTADISTICASJUGADOR_H

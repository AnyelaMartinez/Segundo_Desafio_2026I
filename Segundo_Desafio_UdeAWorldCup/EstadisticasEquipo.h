// EstadisticasEquipo.h
// Estadisticas acumuladas de un equipo durante el torneo.
// Sirven para armar la tabla de posiciones de cada grupo.
//
// Atributos clave:
//  PJ = partidos jugados
//  PG = partidos ganados
//  PE = partidos empatados
//  PP = partidos perdidos
//  GF = goles a favor
//  GC = goles en contra
//  DG = diferencia de goles (GF - GC)
//  Pts = puntos (PG*3 + PE*1)

#ifndef ESTADISTICASEQUIPO_H
#define ESTADISTICASEQUIPO_H

#include <iostream>
#include <string>
using namespace std;

class EstadisticasEquipo {
public:
    int partidosJugados;
    int partidosGanados;
    int partidosEmpatados;
    int partidosPerdidos;
    int golesFavor;
    int golesContra;
    int puntos;

    // Estadisticas extras (las usaremos despues para desempate / fair-play)
    int tarjetasAmarillas;
    int tarjetasRojas;
    int faltas;

    EstadisticasEquipo();
    EstadisticasEquipo(const EstadisticasEquipo& otro);
    EstadisticasEquipo& operator=(const EstadisticasEquipo& otro);

    // Registra el resultado de un partido. golesProp = goles que metio
    // este equipo, golesRiv = goles que le metieron.
    void registrarPartido(int golesProp, int golesRiv);

    // Suma tarjetas (las usaremos para fair-play)
    void registrarTarjetaAmarilla();
    void registrarTarjetaRoja();
    void registrarFalta();

    // Diferencia de goles (GF - GC)
    int getDiferenciaGoles() const;

    int getPuntos() const;
    int getPJ() const;
    int getPG() const;
    int getPE() const;
    int getPP() const;
    int getGF() const;
    int getGC() const;

    // Reinicia todos los contadores (por si se simula varias veces)
    void reiniciar();

    // Compara para tabla de posiciones:
    //  1ro mas puntos
    //  2do mejor diferencia de goles
    //  3ro mas goles a favor
    // Retorna true si "este" va POR ENCIMA de "otro" en la tabla.
    bool vaPorEncimaDe(const EstadisticasEquipo& otro) const;

    // Operador < usado para ordenar (queremos al mejor primero,
    // asi que "a < b" significa "a esta MAS ABAJO que b en la tabla").
    bool operator<(const EstadisticasEquipo& otro) const;

    friend ostream& operator<<(ostream& os, const EstadisticasEquipo& e);
};

#endif // ESTADISTICASEQUIPO_H

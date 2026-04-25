// EstadisticasJugadorPartido.cpp
// Implementacion de la clase EstadisticasJugadorPartido.

#include "EstadisticasJugadorPartido.h"

EstadisticasJugadorPartido::EstadisticasJugadorPartido() {
    goles = 0;
    tarjetasRojas = 0;
    tarjetasAmarillas = 0;
    faltas = 0;
    minutosJugados = 90; // por defecto 90 minutos (tiempo regular)
}

EstadisticasJugadorPartido::EstadisticasJugadorPartido(
                            const EstadisticasJugadorPartido& otro) {
    goles = otro.goles;
    tarjetasRojas = otro.tarjetasRojas;
    tarjetasAmarillas = otro.tarjetasAmarillas;
    faltas = otro.faltas;
    minutosJugados = otro.minutosJugados;
}

// Registra un gol (suma uno).
void EstadisticasJugadorPartido::registrarGol() {
    goles++;
}

// Registra una amarilla. Si ya tenia una amarilla, la segunda
// genera roja automaticamente (segun el requisito del desafio).
void EstadisticasJugadorPartido::registrarAmarilla() {
    tarjetasAmarillas++;
    if (tarjetasAmarillas >= 2) {
        tarjetasRojas = 1;
    }
}

void EstadisticasJugadorPartido::registrarRoja() {
    tarjetasRojas = 1;
}

void EstadisticasJugadorPartido::registrarFalta() {
    faltas++;
}

void EstadisticasJugadorPartido::setMinutos(int m) {
    minutosJugados = m;
}

int EstadisticasJugadorPartido::getGoles() const     { return goles; }
int EstadisticasJugadorPartido::getAmarillas() const { return tarjetasAmarillas; }
int EstadisticasJugadorPartido::getRojas() const     { return tarjetasRojas; }
int EstadisticasJugadorPartido::getFaltas() const    { return faltas; }
int EstadisticasJugadorPartido::getMinutos() const   { return minutosJugados; }

void EstadisticasJugadorPartido::reiniciar() {
    goles = 0;
    tarjetasRojas = 0;
    tarjetasAmarillas = 0;
    faltas = 0;
    minutosJugados = 90;
}

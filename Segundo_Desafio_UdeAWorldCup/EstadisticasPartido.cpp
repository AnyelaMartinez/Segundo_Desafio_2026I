// EstadisticasPartido.cpp
// Implementacion de la clase EstadisticasPartido.

#include "EstadisticasPartido.h"

EstadisticasPartido::EstadisticasPartido() {
    golesLocal = 0;
    golesVisitante = 0;
    totalTarjetasAmarillas = 0;
    totalTarjetasRojas = 0;
    totalFaltas = 0;
    duracionMinutos = 90;
    huboProrroga = false;
    huboPenales = false;
    penalesLocal = -1;     // -1 = "no hubo penales"
    penalesVisitante = -1;
    espectadores = 0;
}

EstadisticasPartido::EstadisticasPartido(const EstadisticasPartido& otro) {
    golesLocal = otro.golesLocal;
    golesVisitante = otro.golesVisitante;
    totalTarjetasAmarillas = otro.totalTarjetasAmarillas;
    totalTarjetasRojas = otro.totalTarjetasRojas;
    totalFaltas = otro.totalFaltas;
    duracionMinutos = otro.duracionMinutos;
    huboProrroga = otro.huboProrroga;
    huboPenales = otro.huboPenales;
    penalesLocal = otro.penalesLocal;
    penalesVisitante = otro.penalesVisitante;
    espectadores = otro.espectadores;
}

void EstadisticasPartido::setGoles(int gl, int gv) {
    golesLocal = gl;
    golesVisitante = gv;
}

void EstadisticasPartido::setProrroga(bool b) {
    huboProrroga = b;
    if (b) {
        duracionMinutos = 120;
    }
}

void EstadisticasPartido::setPenales(int pl, int pv) {
    huboPenales = true;
    penalesLocal = pl;
    penalesVisitante = pv;
}

void EstadisticasPartido::setDuracion(int min) {
    duracionMinutos = min;
}

void EstadisticasPartido::setEspectadores(int n) {
    espectadores = n;
}

void EstadisticasPartido::sumarTarjetaAmarilla() { totalTarjetasAmarillas++; }
void EstadisticasPartido::sumarTarjetaRoja()     { totalTarjetasRojas++; }
void EstadisticasPartido::sumarFalta()           { totalFaltas++; }

int EstadisticasPartido::getGolesLocal() const     { return golesLocal; }
int EstadisticasPartido::getGolesVisitante() const { return golesVisitante; }
int EstadisticasPartido::getTotalTarjetas() const  {
    return totalTarjetasAmarillas + totalTarjetasRojas;
}
int EstadisticasPartido::getTotalFaltas() const    { return totalFaltas; }
int EstadisticasPartido::getDuracion() const       { return duracionMinutos; }
bool EstadisticasPartido::getHuboProrroga() const  { return huboProrroga; }
bool EstadisticasPartido::getHuboPenales() const   { return huboPenales; }

// Determina quien gano:
//  - Si hubo penales, gana quien tenga mas penales.
//  - Si no, comparamos goles.
//  - Si quedo empate y no hubo penales, devolvemos "E".
string EstadisticasPartido::getGanadorSimbolo() const {
    if (huboPenales) {
        if (penalesLocal > penalesVisitante) return "L";
        if (penalesLocal < penalesVisitante) return "V";
        return "E"; // raro, pero por si acaso
    }
    if (golesLocal > golesVisitante) return "L";
    if (golesLocal < golesVisitante) return "V";
    return "E";
}

void EstadisticasPartido::reiniciar() {
    golesLocal = 0;
    golesVisitante = 0;
    totalTarjetasAmarillas = 0;
    totalTarjetasRojas = 0;
    totalFaltas = 0;
    duracionMinutos = 90;
    huboProrroga = false;
    huboPenales = false;
    penalesLocal = -1;
    penalesVisitante = -1;
    espectadores = 0;
}

// Imprime una linea estilo "Resultado: 2-1 (TA=4, TR=0, F=18, dur=90')"
ostream& operator<<(ostream& os, const EstadisticasPartido& e) {
    os << "Resultado: " << e.golesLocal << "-" << e.golesVisitante;
    if (e.huboPenales) {
        os << " (pen " << e.penalesLocal << "-" << e.penalesVisitante << ")";
    }
    os << " | TA=" << e.totalTarjetasAmarillas
       << " TR=" << e.totalTarjetasRojas
       << " F=" << e.totalFaltas
       << " dur=" << e.duracionMinutos << "'";
    if (e.espectadores > 0) {
        os << " esp=" << e.espectadores;
    }
    return os;
}

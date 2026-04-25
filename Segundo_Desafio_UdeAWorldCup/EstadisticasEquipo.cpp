// EstadisticasEquipo.cpp
// Implementacion de la clase EstadisticasEquipo.

#include "EstadisticasEquipo.h"

EstadisticasEquipo::EstadisticasEquipo() {
    partidosJugados = 0;
    partidosGanados = 0;
    partidosEmpatados = 0;
    partidosPerdidos = 0;
    golesFavor = 0;
    golesContra = 0;
    puntos = 0;
    tarjetasAmarillas = 0;
    tarjetasRojas = 0;
    faltas = 0;
}

EstadisticasEquipo::EstadisticasEquipo(const EstadisticasEquipo& otro) {
    partidosJugados   = otro.partidosJugados;
    partidosGanados   = otro.partidosGanados;
    partidosEmpatados = otro.partidosEmpatados;
    partidosPerdidos  = otro.partidosPerdidos;
    golesFavor        = otro.golesFavor;
    golesContra       = otro.golesContra;
    puntos            = otro.puntos;
    tarjetasAmarillas = otro.tarjetasAmarillas;
    tarjetasRojas     = otro.tarjetasRojas;
    faltas            = otro.faltas;
}

// Logica de un partido:
//  - PJ siempre sube en 1
//  - sumamos goles a favor / en contra
//  - si gano -> +3 puntos y PG++
//  - si empato -> +1 punto y PE++
//  - si perdio -> 0 puntos y PP++
void EstadisticasEquipo::registrarPartido(int golesProp, int golesRiv) {
    partidosJugados++;
    golesFavor  += golesProp;
    golesContra += golesRiv;

    if (golesProp > golesRiv) {
        partidosGanados++;
        puntos += 3;
    } else if (golesProp == golesRiv) {
        partidosEmpatados++;
        puntos += 1;
    } else {
        partidosPerdidos++;
    }
}

void EstadisticasEquipo::registrarTarjetaAmarilla() {
    tarjetasAmarillas++;
}

void EstadisticasEquipo::registrarTarjetaRoja() {
    tarjetasRojas++;
}

void EstadisticasEquipo::registrarFalta() {
    faltas++;
}

int EstadisticasEquipo::getDiferenciaGoles() const {
    return golesFavor - golesContra;
}

int EstadisticasEquipo::getPuntos() const { return puntos; }
int EstadisticasEquipo::getPJ() const     { return partidosJugados; }
int EstadisticasEquipo::getPG() const     { return partidosGanados; }
int EstadisticasEquipo::getPE() const     { return partidosEmpatados; }
int EstadisticasEquipo::getPP() const     { return partidosPerdidos; }
int EstadisticasEquipo::getGF() const     { return golesFavor; }
int EstadisticasEquipo::getGC() const     { return golesContra; }

void EstadisticasEquipo::reiniciar() {
    partidosJugados = 0;
    partidosGanados = 0;
    partidosEmpatados = 0;
    partidosPerdidos = 0;
    golesFavor = 0;
    golesContra = 0;
    puntos = 0;
    tarjetasAmarillas = 0;
    tarjetasRojas = 0;
    faltas = 0;
}

// Reglas del Mundial (sin contar enfrentamiento directo, eso ira en Grupo):
//  1) mas puntos
//  2) mejor diferencia de goles
//  3) mas goles a favor
bool EstadisticasEquipo::vaPorEncimaDe(const EstadisticasEquipo& otro) const {
    if (this->puntos != otro.puntos) {
        return this->puntos > otro.puntos;
    }
    int dg1 = this->getDiferenciaGoles();
    int dg2 = otro.getDiferenciaGoles();
    if (dg1 != dg2) {
        return dg1 > dg2;
    }
    return this->golesFavor > otro.golesFavor;
}

// "a < b" significa "a esta MAS ABAJO que b en la tabla" (peor).
// Ojo: es lo opuesto a "a va por encima de b".
bool EstadisticasEquipo::operator<(const EstadisticasEquipo& otro) const {
    return otro.vaPorEncimaDe(*this);
}

// Imprime una linea estilo tabla de posiciones (sin el nombre del equipo,
// porque esta clase no lo conoce).
ostream& operator<<(ostream& os, const EstadisticasEquipo& e) {
    os << "PJ=" << e.partidosJugados
       << " PG=" << e.partidosGanados
       << " PE=" << e.partidosEmpatados
       << " PP=" << e.partidosPerdidos
       << " GF=" << e.golesFavor
       << " GC=" << e.golesContra
       << " DG=" << e.getDiferenciaGoles()
       << " Pts=" << e.puntos;
    return os;
}

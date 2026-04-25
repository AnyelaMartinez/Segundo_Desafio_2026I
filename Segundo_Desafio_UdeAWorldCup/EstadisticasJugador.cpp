// EstadisticasJugador.cpp
// Implementacion de la clase EstadisticasJugador.

#include "EstadisticasJugador.h"

// Constructor por defecto: todos los contadores en cero.
EstadisticasJugador::EstadisticasJugador() {
    partidosJugados = 0;
    goles = 0;
    golesEnMundial = 0;
    minutosJugados = 0;
    asistencias = 0;
    faltas = 0;
    tarjetasAmarillas = 0;
    tarjetasRojas = 0;
}

// Constructor de copia: copia campo a campo.
EstadisticasJugador::EstadisticasJugador(const EstadisticasJugador& otro) {
    partidosJugados = otro.partidosJugados;
    goles = otro.goles;
    golesEnMundial = otro.golesEnMundial;
    minutosJugados = otro.minutosJugados;
    asistencias = otro.asistencias;
    faltas = otro.faltas;
    tarjetasAmarillas = otro.tarjetasAmarillas;
    tarjetasRojas = otro.tarjetasRojas;
}

// Actualiza las estadisticas historicas sumando lo que paso en un partido.
// Tambien suma a golesEnMundial porque este metodo se llama solo para partidos del torneo.
void EstadisticasJugador::actualizarEstadisticas(int goles, int minutos, int amarillas,
                                                 int rojas, int faltas) {
    this->partidosJugados++;
    this->goles += goles;
    this->golesEnMundial += goles;
    this->minutosJugados += minutos;
    this->tarjetasAmarillas += amarillas;
    this->tarjetasRojas += rojas;
    this->faltas += faltas;
}

// --------- Metodos granulares ---------
// Estos los usa la clase Jugador para registrar un evento por vez.

void EstadisticasJugador::actualizarMinutos(int min) {
    minutosJugados += min;
}

void EstadisticasJugador::actualizarGol(bool esMundial) {
    goles++;
    if (esMundial) {
        golesEnMundial++;
    }
}

void EstadisticasJugador::actualizarTarjetaAmarilla() {
    tarjetasAmarillas++;
}

void EstadisticasJugador::actualizarTarjetaRoja() {
    tarjetasRojas++;
}

void EstadisticasJugador::actualizarFalta() {
    faltas++;
}

void EstadisticasJugador::aumentarPartidosJugados() {
    partidosJugados++;
}

void EstadisticasJugador::reiniciar() {
    partidosJugados = 0;
    goles = 0;
    golesEnMundial = 0;
    minutosJugados = 0;
    asistencias = 0;
    faltas = 0;
    tarjetasAmarillas = 0;
    tarjetasRojas = 0;
}

int EstadisticasJugador::getGoles() const          { return goles; }
int EstadisticasJugador::getGolesEnMundial() const { return golesEnMundial; }
int EstadisticasJugador::getMinutos() const        { return minutosJugados; }
int EstadisticasJugador::getTarjetasAmarillas() const { return tarjetasAmarillas; }
int EstadisticasJugador::getTarjetasRojas() const  { return tarjetasRojas; }
int EstadisticasJugador::getFaltas() const         { return faltas; }
int EstadisticasJugador::getPartidosJugados() const { return partidosJugados; }

// Retorna un texto corto con lo mas importante.
string EstadisticasJugador::obtenerResumen() const {
    string r = "PJ=" + to_string(partidosJugados)
             + " Goles=" + to_string(goles)
             + " Min=" + to_string(minutosJugados)
             + " TA=" + to_string(tarjetasAmarillas)
             + " TR=" + to_string(tarjetasRojas)
             + " F=" + to_string(faltas);
    return r;
}

// Imprime el resumen cuando se hace cout << estadistica;
ostream& operator<<(ostream& os, const EstadisticasJugador& e) {
    os << e.obtenerResumen();
    return os;
}

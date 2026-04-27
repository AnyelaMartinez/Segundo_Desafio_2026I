// PosicionGrupo.cpp
// Implementacion de la clase PosicionGrupo.

#include "PosicionGrupo.h"

PosicionGrupo::PosicionGrupo() {
    equipo = nullptr;
    posicion = 0;
    clasificado = false;
}

PosicionGrupo::PosicionGrupo(Equipo* e, int _posicion) {
    equipo = e;
    posicion = _posicion;
    clasificado = false;
}

PosicionGrupo::PosicionGrupo(const PosicionGrupo& otra) {
    equipo = otra.equipo;            // copia el puntero, no el equipo
    posicion = otra.posicion;
    clasificado = otra.clasificado;
}

// operator= explicito (silencia el warning -Wdeprecated-copy)
PosicionGrupo& PosicionGrupo::operator=(const PosicionGrupo& otra) {
    if (this == &otra) return *this;
    equipo = otra.equipo;
    posicion = otra.posicion;
    clasificado = otra.clasificado;
    return *this;
}

PosicionGrupo::~PosicionGrupo() {
    // Nada que liberar: 'equipo' es prestado.
    equipo = nullptr;
}

void PosicionGrupo::setPosicion(int p)    { posicion = p; }
void PosicionGrupo::setClasificado(bool c) { clasificado = c; }
void PosicionGrupo::setEquipo(Equipo* e)  { equipo = e; }

int PosicionGrupo::getPosicion() const     { return posicion; }
bool PosicionGrupo::estaClasificado() const { return clasificado; }
Equipo* PosicionGrupo::getEquipo() const   { return equipo; }

// "a < b" significa que a esta MAS ABAJO en la tabla que b (o sea, peor).
// Reusamos la logica de Equipo::operator<, que a su vez usa
// EstadisticasEquipo (puntos -> DG -> GF).
bool PosicionGrupo::operator<(const PosicionGrupo& otra) const {
    if (equipo == nullptr) return true;       // sin equipo va al final
    if (otra.equipo == nullptr) return false;
    return *equipo < *otra.equipo;
}

bool PosicionGrupo::operator==(const PosicionGrupo& otra) const {
    if (equipo == nullptr || otra.equipo == nullptr) {
        return equipo == otra.equipo;
    }
    return *equipo == *otra.equipo;
}

// Imprime una linea estilo:
//   "1) ARG Argentina | Pts=7 ... [CLASIFICADO]"
ostream& operator<<(ostream& os, const PosicionGrupo& p) {
    os << p.posicion << ") ";
    if (p.equipo != nullptr) {
        os << *p.equipo;   // ya tiene su propio operator<<
    } else {
        os << "(sin equipo)";
    }
    if (p.clasificado) {
        os << " [CLASIFICADO]";
    }
    return os;
}

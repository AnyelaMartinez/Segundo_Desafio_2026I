// Bombo.cpp
// Implementacion de la clase Bombo.

#include "Bombo.h"
#include <cstdlib>   // rand

Bombo::Bombo() {
    numero = 0;
}

Bombo::Bombo(int _numero) {
    numero = _numero;
}

Bombo::Bombo(const Bombo& otro) {
    numero = otro.numero;
    // Copia de punteros (no de equipos)
    for (int i = 0; i < otro.equipos.getTamano(); i++) {
        equipos.agregar(otro.equipos[i]);
    }
}

// operator= explicito (silencia warning -Wdeprecated-copy)
Bombo& Bombo::operator=(const Bombo& otro) {
    if (this == &otro) return *this;
    numero = otro.numero;
    equipos.limpiar();
    for (int i = 0; i < otro.equipos.getTamano(); i++) {
        equipos.agregar(otro.equipos[i]);
    }
    return *this;
}

Bombo::~Bombo() {
    // No borramos equipos: son referencias prestadas.
    // La Lista solo guarda Equipo*, asi que su destructor borra el arreglo
    // de punteros pero no los Equipo en si (eso es de Competicion).
}

void Bombo::setNumero(int n) { numero = n; }
int Bombo::getNumero() const { return numero; }
int Bombo::getCantidad() const { return equipos.getTamano(); }
bool Bombo::estaVacio() const { return equipos.estaVacia(); }

void Bombo::agregarEquipo(Equipo* e) {
    if (e != nullptr) {
        equipos.agregar(e);
    }
}

// Saca un equipo al azar y lo elimina del bombo.
Equipo* Bombo::sacarEquipoAlAzar() {
    int n = equipos.getTamano();
    if (n == 0) return nullptr;
    int idx = rand() % n;
    Equipo* e = equipos[idx];
    equipos.eliminar(idx);
    return e;
}

Equipo* Bombo::getEquipo(int indice) const {
    if (indice < 0 || indice >= equipos.getTamano()) return nullptr;
    return equipos[indice];
}

bool Bombo::operator==(const Bombo& otro) const {
    return numero == otro.numero;
}

ostream& operator<<(ostream& os, const Bombo& b) {
    os << "Bombo #" << b.numero << " (" << b.equipos.getTamano() << " equipos): ";
    for (int i = 0; i < b.equipos.getTamano(); i++) {
        Equipo* e = b.equipos[i];
        if (e != nullptr) {
            if (i > 0) os << ", ";
            os << e->getCodigoFIFA();
        }
    }
    return os;
}

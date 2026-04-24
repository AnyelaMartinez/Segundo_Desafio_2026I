// Federacion.cpp
// Implementacion de la clase Federacion.

#include "Federacion.h"

Federacion::Federacion() {
    nombre = "";
}

Federacion::Federacion(string nombre) {
    this->nombre = nombre;
}

string Federacion::getNombre() const {
    return nombre;
}

bool Federacion::operator==(const Federacion& otra) const {
    return nombre == otra.nombre;
}

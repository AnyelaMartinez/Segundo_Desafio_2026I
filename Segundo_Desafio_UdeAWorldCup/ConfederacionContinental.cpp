// ConfederacionContinental.cpp
// Implementacion de la clase ConfederacionContinental.

#include "ConfederacionContinental.h"

// Constructor por defecto: deja los strings vacios.
ConfederacionContinental::ConfederacionContinental() {
    nombre = "";
    sigla = "";
}

// Constructor con parametros.
ConfederacionContinental::ConfederacionContinental(string nombre, string sigla) {
    this->nombre = nombre;
    this->sigla = sigla;
}

string ConfederacionContinental::getNombre() const {
    return nombre;
}

string ConfederacionContinental::getSigla() const {
    return sigla;
}

// Dos confederaciones son iguales si tienen el mismo nombre.
bool ConfederacionContinental::operator==(const ConfederacionContinental& otra) const {
    return nombre == otra.nombre;
}

// Imprime la confederacion con su sigla, ej: "UEFA".
ostream& operator<<(ostream& os, const ConfederacionContinental& c) {
    os << c.nombre;
    return os;
}

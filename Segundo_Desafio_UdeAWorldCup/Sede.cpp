// Sede.cpp
// Implementacion de la clase Sede.

#include "Sede.h"

Sede::Sede() {
    nombre = "";
    ciudad = "";
    pais = "";
    aforo = 0;
}

Sede::Sede(string _nombre, string _ciudad, string _pais, int _aforo) {
    nombre = _nombre;
    ciudad = _ciudad;
    pais = _pais;
    aforo = _aforo;
}

Sede::Sede(const Sede& otro) {
    nombre = otro.nombre;
    ciudad = otro.ciudad;
    pais = otro.pais;
    aforo = otro.aforo;
}

string Sede::getNombre() const { return nombre; }
string Sede::getCiudad() const { return ciudad; }
string Sede::getPais() const   { return pais; }
int Sede::getAforo() const     { return aforo; }

bool Sede::operator==(const Sede& otro) const {
    return nombre == otro.nombre;
}

ostream& operator<<(ostream& os, const Sede& s) {
    os << s.nombre << " (" << s.ciudad << ", " << s.pais << ")";
    return os;
}

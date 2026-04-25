// Arbitro.cpp
// Implementacion de la clase Arbitro.

#include "Arbitro.h"

Arbitro::Arbitro() {
    nombre = "";
    nacionalidad = "";
    partidosDirigidos = 0;
    tarjetasMostradas = 0;
}

Arbitro::Arbitro(string _nombre, string _nacionalidad) {
    nombre = _nombre;
    nacionalidad = _nacionalidad;
    partidosDirigidos = 0;
    tarjetasMostradas = 0;
}

Arbitro::Arbitro(const Arbitro& otro) {
    nombre = otro.nombre;
    nacionalidad = otro.nacionalidad;
    partidosDirigidos = otro.partidosDirigidos;
    tarjetasMostradas = otro.tarjetasMostradas;
}

void Arbitro::registrarPartidoDirigido() {
    partidosDirigidos++;
}

void Arbitro::registrarTarjeta() {
    tarjetasMostradas++;
}

string Arbitro::getNombre() const         { return nombre; }
string Arbitro::getNacionalidad() const   { return nacionalidad; }
int Arbitro::getPartidosDirigidos() const { return partidosDirigidos; }
int Arbitro::getTarjetasMostradas() const { return tarjetasMostradas; }

bool Arbitro::operator==(const Arbitro& otro) const {
    return nombre == otro.nombre;
}

ostream& operator<<(ostream& os, const Arbitro& a) {
    os << a.nombre << " (" << a.nacionalidad << ")"
       << " [PD=" << a.partidosDirigidos
       << ", T=" << a.tarjetasMostradas << "]";
    return os;
}

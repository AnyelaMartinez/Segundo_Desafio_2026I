// DirectorTecnico.cpp
// Implementacion de la clase DirectorTecnico.

#include "DirectorTecnico.h"

DirectorTecnico::DirectorTecnico() {
    nombre = "";
    nacionalidad = "";
}

DirectorTecnico::DirectorTecnico(string nombre, string nacionalidad) {
    this->nombre = nombre;
    this->nacionalidad = nacionalidad;
}

// Constructor de copia: hace una copia de los strings.
// Como string ya maneja su memoria por dentro, esta copia es segura.
DirectorTecnico::DirectorTecnico(const DirectorTecnico& otro) {
    nombre = otro.nombre;
    nacionalidad = otro.nacionalidad;
}

string DirectorTecnico::getNombre() const {
    return nombre;
}

string DirectorTecnico::getNacionalidad() const {
    return nacionalidad;
}

void DirectorTecnico::setNombre(string n) {
    nombre = n;
}

void DirectorTecnico::setNacionalidad(string n) {
    nacionalidad = n;
}

// Federacion.h
// Federacion nacional de futbol de un pais.
// Ej: "Colombian Football Federation", "Royal Spanish Football Federation".
// Cada equipo pertenece a una federacion nacional.

#ifndef FEDERACION_H
#define FEDERACION_H

#include <string>
using namespace std;

class Federacion {
public:
    string nombre;   // nombre completo de la federacion

    Federacion();
    Federacion(string nombre);

    string getNombre() const;

    // Sobrecarga del == para comparar dos federaciones por nombre.
    bool operator==(const Federacion& otra) const;
};

#endif // FEDERACION_H

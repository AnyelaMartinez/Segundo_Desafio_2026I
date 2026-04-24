// DirectorTecnico.h
// Director tecnico de un equipo nacional.
// Clase simple: solo nombre y nacionalidad.

#ifndef DIRECTORTECNICO_H
#define DIRECTORTECNICO_H

#include <string>
using namespace std;

class DirectorTecnico {
public:
    string nombre;
    string nacionalidad;

    DirectorTecnico();
    DirectorTecnico(string nombre, string nacionalidad);
    DirectorTecnico(const DirectorTecnico& otro); // constructor de copia

    string getNombre() const;
    string getNacionalidad() const;

    void setNombre(string n);
    void setNacionalidad(string n);
};

#endif // DIRECTORTECNICO_H

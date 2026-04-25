// Sede.h
// Representa un estadio donde se juegan partidos del Mundial.
// Es una clase simple, sin memoria dinamica.

#ifndef SEDE_H
#define SEDE_H

#include <iostream>
#include <string>
using namespace std;

class Sede {
public:
    string nombre;     // ej: "Estadio Azteca"
    string ciudad;     // ej: "Ciudad de Mexico"
    string pais;       // ej: "Mexico"
    int aforo;         // capacidad de espectadores

    Sede();
    Sede(string _nombre, string _ciudad, string _pais, int _aforo);
    Sede(const Sede& otro);   // constructor copia

    // Getters
    string getNombre() const;
    string getCiudad() const;
    string getPais() const;
    int getAforo() const;

    // Dos sedes son iguales si tienen el mismo nombre.
    bool operator==(const Sede& otro) const;

    // Para imprimir: "Estadio Azteca (Ciudad de Mexico, Mexico)"
    friend ostream& operator<<(ostream& os, const Sede& s);
};

#endif // SEDE_H

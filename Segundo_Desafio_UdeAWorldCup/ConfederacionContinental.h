// ConfederacionContinental.h
// Representa una confederacion continental (UEFA, CONMEBOL, etc.).
// Cada equipo pertenece a una y es clave para la regla de sorteo:
// no puede haber dos equipos de la misma confederacion en un grupo,
// salvo UEFA que permite hasta dos.

#ifndef CONFEDERACIONCONTINENTAL_H
#define CONFEDERACIONCONTINENTAL_H

#include <string>
#include <iostream>
using namespace std;

class ConfederacionContinental {
public:
    // Los dejo publicos para leerlos facil desde otras clases
    // (setters no los pide el diagrama, por eso directos).
    string nombre;   // "UEFA", "CONMEBOL", "CONCACAF", "CAF", "AFC", "OFC"
    string sigla;    // en este proyecto uso el mismo texto como sigla

    ConfederacionContinental();
    ConfederacionContinental(string nombre, string sigla);

    string getNombre() const;
    string getSigla() const;

    // Sobrecarga del operador ==: sirve para comparar dos confederaciones.
    // Lo usaremos en el sorteo para contar cuantos equipos hay por conf. en un grupo.
    bool operator==(const ConfederacionContinental& otra) const;

    // Funcion amiga: permite hacer cout << confederacion; directamente.
    friend ostream& operator<<(ostream& os, const ConfederacionContinental& c);
};

#endif // CONFEDERACIONCONTINENTAL_H

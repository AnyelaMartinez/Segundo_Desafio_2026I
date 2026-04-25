// Arbitro.h
// Representa al arbitro principal de un partido.
// Sin memoria dinamica, asi que regla de los 3 no aplica.

#ifndef ARBITRO_H
#define ARBITRO_H

#include <iostream>
#include <string>
using namespace std;

class Arbitro {
public:
    string nombre;
    string nacionalidad;

    // Llevamos cuenta de cuantos partidos ha dirigido durante el torneo
    // (sirve para no dejarlo dirigir muchos seguidos en la simulacion).
    int partidosDirigidos;
    int tarjetasMostradas;  // amarillas + rojas, total acumulado

    Arbitro();
    Arbitro(string _nombre, string _nacionalidad);
    Arbitro(const Arbitro& otro);

    // Cuando el arbitro termina de dirigir un partido se llama a esto
    void registrarPartidoDirigido();

    // Cuando el arbitro saca una tarjeta (amarilla o roja)
    void registrarTarjeta();

    string getNombre() const;
    string getNacionalidad() const;
    int getPartidosDirigidos() const;
    int getTarjetasMostradas() const;

    // Dos arbitros se consideran iguales si tienen el mismo nombre
    // (asumimos que no hay dos con el mismo nombre exacto en el torneo).
    bool operator==(const Arbitro& otro) const;

    friend ostream& operator<<(ostream& os, const Arbitro& a);
};

#endif // ARBITRO_H

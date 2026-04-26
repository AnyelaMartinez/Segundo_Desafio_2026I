// Bombo.h
// Representa uno de los 4 bombos del sorteo del Mundial.
// Cada bombo tiene 12 equipos (TOTAL_EQUIPOS / TOTAL_BOMBOS).
//
// Los bombos se llenan segun el ranking FIFA:
//   Bombo 1: equipos rankeados del 1 al 12
//   Bombo 2: del 13 al 24
//   Bombo 3: del 25 al 36
//   Bombo 4: del 37 al 48
//
// Durante el sorteo se "saca" un equipo al azar de cada bombo y se asigna
// a un grupo. Por eso el bombo guarda PUNTEROS prestados (no es dueno).

#ifndef BOMBO_H
#define BOMBO_H

#include <iostream>
#include "tipos.h"
#include "Lista.h"
#include "Equipo.h"

using namespace std;

class Bombo {
public:
    int numero;                  // 1, 2, 3 o 4
    Lista<Equipo*> equipos;      // referencias prestadas (no se borran aqui)

    Bombo();
    Bombo(int _numero);
    Bombo(const Bombo& otro);    // copia los punteros, no los equipos
    ~Bombo();

    void setNumero(int n);
    int getNumero() const;
    int getCantidad() const;
    bool estaVacio() const;

    // Agrega un equipo al bombo (toma solo el puntero)
    void agregarEquipo(Equipo* e);

    // Saca un equipo al azar del bombo y lo retorna.
    // El equipo queda fuera del bombo despues de esto.
    // Si el bombo esta vacio devuelve nullptr.
    Equipo* sacarEquipoAlAzar();

    // Permite consultar el equipo en una posicion (sin sacarlo)
    Equipo* getEquipo(int indice) const;

    bool operator==(const Bombo& otro) const;
    friend ostream& operator<<(ostream& os, const Bombo& b);
};

#endif // BOMBO_H

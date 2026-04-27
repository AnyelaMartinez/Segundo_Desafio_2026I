// PosicionGrupo.h
// Representa UNA fila de la tabla de posiciones de un grupo.
// Une un equipo con su puesto (1-4) y si se clasifico a la siguiente ronda.
//
// El equipo lo guardamos como puntero (referencia prestada): el dueno real
// del Equipo es la clase Competicion, no nosotros.

#ifndef POSICIONGRUPO_H
#define POSICIONGRUPO_H

#include <iostream>
#include <string>
#include "Equipo.h"

using namespace std;

class PosicionGrupo {
public:
    Equipo* equipo;       // referencia (no se borra aqui)
    int posicion;         // 1..4 dentro del grupo
    bool clasificado;     // true si pasa a la siguiente ronda

    // Constructores
    PosicionGrupo();
    PosicionGrupo(Equipo* e, int _posicion);
    PosicionGrupo(const PosicionGrupo& otra);
    PosicionGrupo& operator=(const PosicionGrupo& otra);

    // No hay memoria dinamica propia, asi que destructor por defecto
    // serviria. Igual lo dejamos explicito por claridad.
    ~PosicionGrupo();

    // Setters
    void setPosicion(int p);
    void setClasificado(bool c);
    void setEquipo(Equipo* e);

    // Getters
    int getPosicion() const;
    bool estaClasificado() const;
    Equipo* getEquipo() const;

    // Para ordenar la tabla: la clase Grupo va a usar este operador
    // (basicamente lo redirige al < de los equipos, que a su vez se basa
    //  en EstadisticasEquipo: puntos -> diferencia de goles -> goles a favor).
    bool operator<(const PosicionGrupo& otra) const;

    // Dos posiciones se consideran iguales si referencian al mismo equipo.
    bool operator==(const PosicionGrupo& otra) const;

    friend ostream& operator<<(ostream& os, const PosicionGrupo& p);
};

#endif // POSICIONGRUPO_H

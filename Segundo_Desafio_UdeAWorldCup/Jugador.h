// Jugador.h
// Representa a un jugador de una seleccion del Mundial.
// Cada jugador tiene por dentro un puntero a EstadisticasJugador,
// para practicar memoria dinamica (como pide el desafio).

#ifndef JUGADOR_H
#define JUGADOR_H

#include <iostream>
#include <string>
#include "EstadisticasJugador.h"
using namespace std;

class Jugador {
public:
    int id;                 // identificador interno
    string nombre;
    string apellido;
    int numeroCamiseta;     // dorsal (1..99)
    bool activo;            // false si fue expulsado (roja) y no puede jugar el proximo

    // Puntero a las estadisticas. Se crea con 'new' en el constructor
    // y se libera en el destructor. Por eso necesitamos regla de los 3.
    EstadisticasJugador* estadisticas;

    // Constructores
    Jugador();
    Jugador(int _id, string _nombre, string _apellido, int _dorsal);
    Jugador(const Jugador& otro);   // copia profunda
    ~Jugador();

    // Getters / setters
    int getId() const;
    string getNombre() const;
    string getApellido() const;
    int getDorsal() const;
    bool estaActivo() const;
    void setActivo(bool a);

    // Registrar eventos durante un partido
    // (aqui solo actualizamos las estadisticas historicas)
    void registrarMinutos(int min);
    void registrarGol(bool esMundial); // esMundial=true si es partido de Mundial
    void registrarTarjeta(bool esRoja);
    void registrarFalta();

    // Si recibe roja -> queda inactivo (no juega el proximo partido)
    void fueExpulsado();

    // Operador == : dos jugadores son iguales si tienen el mismo id
    bool operator==(const Jugador& otro) const;

    // Operador < : compara por goles en el Mundial (para rankings)
    bool operator<(const Jugador& otro) const;

    // Impresion amigable del jugador
    friend ostream& operator<<(ostream& os, const Jugador& j);
};

#endif // JUGADOR_H

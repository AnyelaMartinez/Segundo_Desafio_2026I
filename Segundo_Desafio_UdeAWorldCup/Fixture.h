// Fixture.h
// Clase que organiza los partidos en un calendario (dias del Mundial).
// Reglas que tiene que respetar:
//   - Maximo 4 partidos por dia (la opcion del enunciado).
//   - Cada equipo debe descansar al menos 3 dias entre partido y partido.
//
// La clase NO simula los partidos, solo decide en que dia se juega cada uno.
// El simular y registrar resultados queda para Competicion.
//
// Los Partido* aqui dentro son PRESTADOS: el Fixture no los crea ni los borra.
// Quien los crea y los destruye es la Competicion (o el Grupo, segun el caso).

#ifndef FIXTURE_H
#define FIXTURE_H

#include <iostream>
#include <string>
#include "Lista.h"
#include "Partido.h"
#include "Equipo.h"
using namespace std;

class Fixture {
public:
    string nombre;             // p.ej. "Fixture Fase de Grupos" o "Fixture R16"
    int diaActualMaximo;       // ultimo dia usado hasta ahora
    int maxPartidosPorDia;     // por defecto 4
    int diasDescansoMinimo;    // por defecto 3

    // Lista de TODOS los partidos que ya estan agendados.
    // Son punteros prestados: NO hacemos delete aqui.
    Lista<Partido*> partidosAgendados;

    Fixture();
    Fixture(string _nombre);
    Fixture(const Fixture& otro);
    ~Fixture();

    // Setters/getters basicos
    void setNombre(string n);
    string getNombre() const;
    int getDiaActualMaximo() const;
    int getCantidadPartidos() const;
    void setMaxPartidosPorDia(int m);
    void setDiasDescansoMinimo(int d);

    // Agenda un partido en el primer dia disponible que cumpla las dos reglas.
    // Si lo logra, le pone el dia al partido con setDia() y devuelve true.
    bool agendarPartido(Partido* p);

    // Agenda un partido pero forzando un dia minimo (util en eliminatorias:
    // los partidos de R16 deben empezar despues del ultimo dia de grupos).
    bool agendarPartidoDesde(Partido* p, int diaMinimo);

    // Devuelve cuantos partidos ya estan agendados en un dia X.
    int contarPartidosEnDia(int dia) const;

    // Devuelve true si el equipo eq ya jugo el dia X
    // (sirve para revisar el descanso de los otros dias).
    bool equipoJuegaEseDia(Equipo* eq, int dia) const;

    // Devuelve el ultimo dia en que el equipo eq tiene un partido agendado.
    // Si no tiene ninguno, devuelve 0.
    int ultimoDiaDeEquipo(Equipo* eq) const;

    // Imprime el calendario completo, dia por dia.
    void mostrarCalendario() const;

    // Vacia la lista (sin borrar los Partido*, que son prestados).
    void limpiar();

    // Operadores
    Fixture& operator=(const Fixture& otro);
    bool operator==(const Fixture& otro) const;

    friend ostream& operator<<(ostream& os, const Fixture& f);
};

#endif // FIXTURE_H

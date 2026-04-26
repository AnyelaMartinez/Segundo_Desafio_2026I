// Grupo.h
// Representa un grupo del Mundial (A, B, C, ..., L). Son 12 grupos.
// Cada grupo tiene:
//   - 4 equipos (referencias prestadas)
//   - 6 partidos (cada equipo juega contra los otros 3): Lista<Partido*>
//   - tabla de posiciones (Lista<PosicionGrupo>)
//
// El Grupo NO es dueno de los Equipo* ni de los Partido*. Esos los crea
// la clase Competicion. Aqui solo guardamos referencias.

#ifndef GRUPO_H
#define GRUPO_H

#include <iostream>
#include <string>
#include "tipos.h"
#include "Lista.h"
#include "Equipo.h"
#include "Partido.h"
#include "PosicionGrupo.h"

using namespace std;

class Grupo {
public:
    char letra;                           // 'A', 'B', ... 'L'
    Lista<Equipo*> equipos;               // referencias prestadas
    Lista<Partido*> partidos;             // referencias prestadas
    Lista<PosicionGrupo> tablaPosiciones; // se construye al final

    Grupo();
    Grupo(char _letra);
    Grupo(const Grupo& otro);             // copia los punteros (no los objetos)
    ~Grupo();

    void setLetra(char l);
    char getLetra() const;

    // Agregar referencias
    void agregarEquipo(Equipo* e);
    void agregarPartido(Partido* p);

    // Cantidades
    int getCantidadEquipos() const;
    int getCantidadPartidos() const;

    // Construye la tabla de posiciones a partir de las estadisticas
    // ya acumuladas en cada Equipo del grupo. Ordena de mejor a peor.
    void calcularTabla();

    // Marca como clasificados a los primeros 'n' de la tabla
    // (en el Mundial 2026 son los 2 primeros de cada grupo).
    void marcarClasificados(int n);

    // Devuelve el equipo que quedo en la posicion dada (1..4).
    // Si todavia no se calculo la tabla, devuelve nullptr.
    Equipo* getEquipoEnPosicion(int posicion) const;

    // Devuelve la cantidad de equipos clasificados (despues de marcarlos).
    int getCantidadClasificados() const;

    // Imprime la tabla de posiciones
    void mostrarTabla() const;

    // Comparacion por letra
    bool operator==(const Grupo& otro) const;

    friend ostream& operator<<(ostream& os, const Grupo& g);
};

#endif // GRUPO_H

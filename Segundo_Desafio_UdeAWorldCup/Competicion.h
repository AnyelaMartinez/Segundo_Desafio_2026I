// Competicion.h
// Clase central del proyecto: representa al Mundial completo.
//
// Responsabilidades:
//   - Guardar los 48 equipos clasificados.
//   - Llenar los 4 bombos por ranking FIFA.
//   - Hacer el sorteo: armar 12 grupos de 4 equipos (A..L)
//     respetando que no haya dos equipos de la misma confederacion en
//     un mismo grupo (excepto UEFA, que tiene tantos que se permite).
//   - Llevar las listas de sedes y arbitros (creadas hardcodeadas, suficiente
//     para el desafio).
//   - Mas adelante: simular fase de grupos, eliminatorias, mostrar
//     estadisticas finales (esto se hara en las Partes B y C).
//
// Memoria:
//   - equipos: PRESTADOS desde main (g_equipos los crea y los destruye main).
//   - bombos: por valor (no son punteros).
//   - grupos: PROPIOS (Competicion los crea y los borra en el destructor).
//   - sedes y arbitros: PROPIOS (creados con new, borrados en destructor).
//   - partidos: PROPIOS (se crearan en Parte B durante la simulacion).

#ifndef COMPETICION_H
#define COMPETICION_H

#include <iostream>
#include <string>
#include "Lista.h"
#include "tipos.h"
#include "Equipo.h"
#include "Bombo.h"
#include "Grupo.h"
#include "Sede.h"
#include "Arbitro.h"
#include "Partido.h"
#include "Fixture.h"
using namespace std;

class Competicion {
public:
    string nombre;                  // ej: "FIFA World Cup 2026"
    int anio;                       // ej: 2026

    // Equipos prestados (NO se borran aqui)
    Lista<Equipo*> equipos;

    // Los 4 bombos del sorteo (por valor, son chicos)
    Bombo bombos[4];

    // 12 grupos (A..L) - propios
    Lista<Grupo*> grupos;

    // Sedes y arbitros del torneo - propios
    Lista<Sede*> sedes;
    Lista<Arbitro*> arbitros;

    // Lista global de TODOS los partidos del Mundial (grupos + eliminatorias).
    // Se llena en Parte B; aqui solo la dejamos preparada.
    Lista<Partido*> partidos;

    // Calendario del torneo (sera usado en Parte B/C)
    Fixture fixture;

    // Banderas de control
    bool sorteoRealizado;

    Competicion();
    Competicion(string _nombre, int _anio);
    Competicion(const Competicion& otro); // (copia "ligera", ver .cpp)
    ~Competicion();

    // Configuracion inicial
    void setEquipos(const Lista<Equipo*>& fuente); // copia los punteros (prestados)
    void cargarSedes();     // hardcodea 16 sedes del Mundial 2026
    void cargarArbitros();  // hardcodea ~12 arbitros
    void cargarBombos();    // separa los 48 equipos en 4 bombos por ranking

    // Sorteo
    // Toma un equipo de cada bombo y lo asigna a cada grupo (A..L).
    // Respeta: no dos equipos de la misma confederacion en el mismo grupo
    // (salvo UEFA, que se permiten 2).
    bool realizarSorteo();

    // ========= Parte B: fase de grupos =========

    // Si algun equipo no tiene jugadores cargados, le crea 11 (ficticios)
    // para poder simular partidos. (El CSV no trae jugadores, asi que es
    // necesario antes de la primera simulacion.)
    void asegurarJugadoresEnEquipos();

    // Recorre los 12 grupos y crea los 6 partidos por grupo (round-robin).
    // Asigna sede y arbitro al azar y los agrega al fixture.
    void generarPartidosFaseGrupos();

    // Simula todos los partidos de FASE_GRUPOS en orden cronologico (por dia).
    // Cada simular() actualiza las estadisticas de los equipos.
    void simularFaseGrupos();

    // Calcula la tabla de cada grupo y marca los 2 primeros como clasificados.
    void construirTablasYClasificar();

    // Reportes de la fase de grupos
    void mostrarResultadosFaseGrupos() const;
    void mostrarTablasGrupos() const;

    // Banderas de control extra
    bool partidosGrupoGenerados;
    bool gruposSimulados;

    // ========= Parte C: eliminatorias =========

    // Lista de los 32 equipos que entran a R16 (24 + 8 mejores terceros).
    // Son punteros prestados (los Equipo viven en 'equipos').
    Lista<Equipo*> clasificadosR16;

    // Resultados finales del torneo (prestados, no se borran aqui)
    Equipo* campeon;
    Equipo* subcampeon;
    Equipo* tercero;
    Equipo* cuarto;

    // Banderas de las eliminatorias
    bool eliminatoriasGeneradas;
    bool eliminatoriasSimuladas;

    // Selecciona los 32 equipos para R16: los 2 primeros de cada grupo
    // y los 8 mejores terceros. Devuelve true si quedan 32.
    bool seleccionarClasificadosR16();

    // Crea los 16 partidos de R16 emparejando los 32 clasificados
    // de forma simple y los agenda en el fixture (despues del ultimo
    // dia de fase de grupos).
    void generarPartidosR16();

    // Simula todos los partidos de la etapa dada y devuelve la lista
    // de ganadores (en el orden de los partidos).
    void simularEtapa(Etapa etapa, Lista<Equipo*>& ganadores);

    // Crea los partidos de la siguiente etapa a partir de la lista
    // de ganadores. Los agenda despues del ultimo dia agendado.
    void crearPartidosSiguienteEtapa(Etapa etapa, const Lista<Equipo*>& participantes);

    // Crea el partido por el tercer puesto a partir de los 2 perdedores
    // de semifinales.
    void crearPartidoTercerPuesto(Equipo* perdedorSF1, Equipo* perdedorSF2);

    // Orquesta toda la fase eliminatoria de principio a fin:
    // R16 -> R8 -> QF -> SF -> Tercer Puesto -> Final
    void simularEliminatorias();

    // Reportes
    void mostrarBracket() const;       // todas las eliminatorias por etapa
    void mostrarPodio() const;          // campeon, sub, 3ro, 4to
    void mostrarEstadisticasFinales() const; // top goleador, mas tarjetas, etc.

    // Consultas / muestra
    int getCantidadEquipos() const;
    int getCantidadGrupos() const;
    bool getSorteoRealizado() const;
    Grupo* getGrupo(int indice);
    Grupo* getGrupoPorLetra(char letra);
    void mostrarBombos() const;
    void mostrarGrupos() const;

    // Vacia la competencia (libera grupos, partidos, sedes, arbitros).
    void limpiar();

    // Operadores
    Competicion& operator=(const Competicion& otro);
    bool operator==(const Competicion& otro) const;
    friend ostream& operator<<(ostream& os, const Competicion& c);

private:
    // Heuristica auxiliar: ¿puedo meter este equipo en este grupo
    // sin violar la regla de confederaciones?
    bool grupoAceptaEquipo(Grupo* g, Equipo* eq) const;

    // Asigna un equipo del bombo b a algun grupo valido.
    // Devuelve true si pudo, false si no encontro un grupo.
    bool asignarDesdeBombo(Bombo& b);
};

#endif // COMPETICION_H

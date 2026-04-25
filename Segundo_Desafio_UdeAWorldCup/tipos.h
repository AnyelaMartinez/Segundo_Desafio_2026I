// tipos.h

#ifndef TIPOS_H
#define TIPOS_H

// Enum con las etapas del torneo (fase de grupos + eliminatorias)
// Se usa en la clase Partido y en Fixture para saber en que ronda esta.
enum Etapa {
    FASE_GRUPOS,
    R16,              // dieciseisavos de final (32 equipos)
    R8,               // octavos de final
    QF,               // cuartos de final
    SF,               // semifinales
    TERCER_PUESTO,
    FINAL_COPA
};

// Constantes del mundial 2026 (no cambian durante la ejecucion)
const int TOTAL_EQUIPOS      = 48;
const int TOTAL_GRUPOS       = 12;
const int EQUIPOS_POR_GRUPO  = 4;
const int JUGADORES_POR_EQUIPO = 26;
const int JUGADORES_CONVOCADOS = 11;
const int TOTAL_BOMBOS       = 4;
const int EQUIPOS_POR_BOMBO  = 12;
const int MAX_PARTIDOS_POR_DIA = 4;
const int DIAS_DESCANSO      = 3;

#endif // TIPOS_H

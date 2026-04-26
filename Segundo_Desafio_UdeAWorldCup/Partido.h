// Partido.h
// Representa un partido del Mundial.
// Es el corazon de la simulacion: aqui se calcula el resultado con la
// formula de Poisson que pide el desafio.
//
// Composicion:
//   - 2 ParticipacionEquipo* (local y visitante) - MEMORIA PROPIA
//   - Sede* y Arbitro*                            - REFERENCIAS prestadas
//   - EstadisticasPartido (por valor)             - composicion fuerte
//   - Etapa (enum) y un id/numero de dia          - datos simples

#ifndef PARTIDO_H
#define PARTIDO_H

#include <iostream>
#include <string>
#include "tipos.h"
#include "Sede.h"
#include "Arbitro.h"
#include "EstadisticasPartido.h"
#include "ParticipacionEquipo.h"
#include "Equipo.h"

using namespace std;

class Partido {
public:
    int id;                              // numero del partido (1, 2, 3...)
    Etapa etapa;                         // FASE_GRUPOS, R16, QF, SF, etc.
    int diaTorneo;                       // dia 1, dia 2... del calendario

    ParticipacionEquipo* local;          // memoria propia (la borramos)
    ParticipacionEquipo* visitante;      // memoria propia (la borramos)

    Sede* sede;                          // referencia (no se borra aqui)
    Arbitro* arbitro;                    // referencia (no se borra aqui)

    EstadisticasPartido estadisticas;    // composicion fuerte
    bool yaSimulado;                     // true cuando termino

    // Constructores
    Partido();
    Partido(int _id, Etapa _etapa, Equipo* eqLocal, Equipo* eqVisitante);
    Partido(const Partido& otro);   // copia profunda
    ~Partido();

    // Setters
    void setSede(Sede* s);
    void setArbitro(Arbitro* a);
    void setDia(int d);
    void setEtapa(Etapa e);

    // Getters
    int getId() const;
    Etapa getEtapa() const;
    int getDia() const;
    bool fueSimulado() const;
    ParticipacionEquipo* getLocal() const;
    ParticipacionEquipo* getVisitante() const;
    EstadisticasPartido& getEstadisticas();
    const EstadisticasPartido& getEstadisticas() const;

    // Devuelve el equipo ganador. Si hay empate (solo en grupos), nullptr.
    // En eliminatorias siempre tiene que devolver alguien (por penales).
    Equipo* getEquipoGanador() const;
    Equipo* getEquipoPerdedor() const;

    // -------- LO IMPORTANTE: SIMULACION --------
    // 1) Convoca 11 titulares de cada equipo.
    // 2) Calcula lambda local y lambda visitante con la formula de Poisson.
    // 3) Saca el numero de goles de cada lado (variable aleatoria Poisson).
    // 4) Si es eliminatoria y hay empate, aplica prorroga y penales.
    // 5) Vuelca las estadisticas a los equipos y a los jugadores.
    void simular();

    // Devuelve el nombre legible de la etapa: "Fase de Grupos", "R16", etc.
    string getNombreEtapa() const;

    // Compara por id
    bool operator==(const Partido& otro) const;

    friend ostream& operator<<(ostream& os, const Partido& p);

private:
    // Helpers de simulacion (privados porque son detalles internos).

    // Calcula λ con la formula de Poisson:
    //   λ = mu * (gfa/mu)^alpha * (gcb/mu)^beta
    double calcularLambda(double gfaPropio, double gcbRival) const;

    // Genera un valor aleatorio de una distribucion Poisson(λ).
    // Usa el algoritmo simple de Knuth (suficiente para esta simulacion).
    int muestrearPoisson(double lambda) const;

    // En eliminatorias: si hay empate al final, simulamos penales.
    void simularPenales();

    // Reparte los goles del equipo entre sus convocados (al azar).
    void repartirGoles(ParticipacionEquipo* lado, int golesEquipo);

    // Vuelca todo al final del partido (equipo + jugadores).
    void cerrarPartido();
};

#endif // PARTIDO_H

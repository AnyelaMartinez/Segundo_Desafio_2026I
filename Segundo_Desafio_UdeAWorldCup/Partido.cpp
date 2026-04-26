// Partido.cpp
// Implementacion de la clase Partido.

#include "Partido.h"
#include <cmath>      // pow, exp
#include <cstdlib>    // rand
using namespace std;

// Constantes de la formula del desafio
static const double MU    = 1.35;  // promedio global de goles
static const double ALPHA = 0.6;   // peso del ataque propio
static const double BETA  = 0.4;   // peso de la defensa rival

// ===== Constructores =====

Partido::Partido() {
    id = 0;
    etapa = FASE_GRUPOS;
    diaTorneo = 0;
    local = nullptr;
    visitante = nullptr;
    sede = nullptr;
    arbitro = nullptr;
    yaSimulado = false;
}

Partido::Partido(int _id, Etapa _etapa, Equipo* eqLocal, Equipo* eqVisitante) {
    id = _id;
    etapa = _etapa;
    diaTorneo = 0;
    sede = nullptr;
    arbitro = nullptr;
    yaSimulado = false;

    // Creamos las dos participaciones (memoria propia)
    local     = new ParticipacionEquipo(eqLocal, true);
    visitante = new ParticipacionEquipo(eqVisitante, false);

    // Y les decimos que pertenecen a este partido
    local->setPartido(this);
    visitante->setPartido(this);
}

// Constructor copia: copia profunda de las participaciones.
// Sede y Arbitro se copian solo el puntero (son referencias prestadas).
Partido::Partido(const Partido& otro) {
    id = otro.id;
    etapa = otro.etapa;
    diaTorneo = otro.diaTorneo;
    sede = otro.sede;
    arbitro = otro.arbitro;
    estadisticas = otro.estadisticas;
    yaSimulado = otro.yaSimulado;

    if (otro.local != nullptr) {
        local = new ParticipacionEquipo(*otro.local);
        local->setPartido(this);
    } else {
        local = nullptr;
    }
    if (otro.visitante != nullptr) {
        visitante = new ParticipacionEquipo(*otro.visitante);
        visitante->setPartido(this);
    } else {
        visitante = nullptr;
    }
}

Partido::~Partido() {
    // Borramos las participaciones (las creamos nosotros con new)
    if (local != nullptr) {
        delete local;
        local = nullptr;
    }
    if (visitante != nullptr) {
        delete visitante;
        visitante = nullptr;
    }
    // OJO: NO borramos sede ni arbitro, no son nuestros.
}

// ===== Setters / getters =====

void Partido::setSede(Sede* s)       { sede = s; }
void Partido::setArbitro(Arbitro* a) { arbitro = a; }
void Partido::setDia(int d)          { diaTorneo = d; }
void Partido::setEtapa(Etapa e)      { etapa = e; }

int Partido::getId() const     { return id; }
Etapa Partido::getEtapa() const { return etapa; }
int Partido::getDia() const    { return diaTorneo; }
bool Partido::fueSimulado() const { return yaSimulado; }
ParticipacionEquipo* Partido::getLocal() const     { return local; }
ParticipacionEquipo* Partido::getVisitante() const { return visitante; }
EstadisticasPartido& Partido::getEstadisticas()             { return estadisticas; }
const EstadisticasPartido& Partido::getEstadisticas() const { return estadisticas; }

// Devuelve el equipo ganador segun la EstadisticasPartido (que respeta los penales).
Equipo* Partido::getEquipoGanador() const {
    if (!yaSimulado) return nullptr;
    string g = estadisticas.getGanadorSimbolo();
    if (g == "L") return (local != nullptr) ? local->getEquipo() : nullptr;
    if (g == "V") return (visitante != nullptr) ? visitante->getEquipo() : nullptr;
    return nullptr; // empate (solo posible en fase de grupos)
}

Equipo* Partido::getEquipoPerdedor() const {
    if (!yaSimulado) return nullptr;
    string g = estadisticas.getGanadorSimbolo();
    if (g == "L") return (visitante != nullptr) ? visitante->getEquipo() : nullptr;
    if (g == "V") return (local != nullptr) ? local->getEquipo() : nullptr;
    return nullptr;
}

string Partido::getNombreEtapa() const {
    switch (etapa) {
        case FASE_GRUPOS:   return "Fase de Grupos";
        case R16:           return "Dieciseisavos";
        case R8:            return "Octavos";
        case QF:            return "Cuartos";
        case SF:            return "Semifinal";
        case TERCER_PUESTO: return "Tercer Puesto";
        case FINAL_COPA:    return "Final";
    }
    return "?";
}

// ====== HELPERS DE SIMULACION ======

// Formula del desafio:
//   λ = mu * (gfa/mu)^alpha * (gcb/mu)^beta
double Partido::calcularLambda(double gfaPropio, double gcbRival) const {
    // Si los promedios son 0 (no se cargo bien) usamos mu por defecto
    if (gfaPropio <= 0.0) gfaPropio = MU;
    if (gcbRival  <= 0.0) gcbRival  = MU;

    double parteAtaque  = pow(gfaPropio / MU, ALPHA);
    double parteDefensa = pow(gcbRival  / MU, BETA);
    return MU * parteAtaque * parteDefensa;
}

// Algoritmo de Knuth para muestrear de una Poisson(λ):
//  L = e^-λ
//  k = 0, p = 1
//  Mientras p > L: k++, p *= U(0,1)
//  Devolver k - 1
int Partido::muestrearPoisson(double lambda) const {
    if (lambda <= 0.0) return 0;
    double L = exp(-lambda);
    int k = 0;
    double p = 1.0;
    do {
        k++;
        // numero aleatorio entre 0 y 1
        double u = (double)rand() / (double)RAND_MAX;
        p *= u;
    } while (p > L);
    return k - 1;
}

// Reparte los goles del equipo entre los convocados (al azar).
void Partido::repartirGoles(ParticipacionEquipo* lado, int golesEquipo) {
    if (lado == nullptr || golesEquipo <= 0) return;
    int n = lado->getCantidadConvocados();
    if (n == 0) return;

    for (int g = 0; g < golesEquipo; g++) {
        int idx = rand() % n;       // jugador aleatorio
        lado->registrarGolDeJugador(idx);
    }
}

// Simula penales en eliminatorias cuando quedan empatados tras la prorroga.
// Versionm muy simple: cada lado tira hasta 5 penales y cada uno entra
// con probabilidad 0.75.  Si quedan empatados, muerte subita (1 vs 1).
void Partido::simularPenales() {
    int penL = 0, penV = 0;
    for (int i = 0; i < 5; i++) {
        double u1 = (double)rand() / (double)RAND_MAX;
        double u2 = (double)rand() / (double)RAND_MAX;
        if (u1 < 0.75) penL++;
        if (u2 < 0.75) penV++;
    }
    // Muerte subita: tirando de a uno hasta que se rompa el empate.
    while (penL == penV) {
        double u1 = (double)rand() / (double)RAND_MAX;
        double u2 = (double)rand() / (double)RAND_MAX;
        bool ml = (u1 < 0.75);
        bool mv = (u2 < 0.75);
        if (ml) penL++;
        if (mv) penV++;
    }
    estadisticas.setPenales(penL, penV);
}

// ====== EL METODO PRINCIPAL ======

void Partido::simular() {
    if (yaSimulado) return;
    if (local == nullptr || visitante == nullptr) return;

    Equipo* eqL = local->getEquipo();
    Equipo* eqV = visitante->getEquipo();
    if (eqL == nullptr || eqV == nullptr) return;

    // 1) Reactivamos a todos los jugadores antes de convocar
    eqL->reactivarJugadores();
    eqV->reactivarJugadores();

    // 2) Convocamos a los 11 titulares de cada lado
    local->convocarTitulares();
    visitante->convocarTitulares();

    // 3) Calculamos lambdas con la formula de Poisson
    double lambdaL = calcularLambda(eqL->getPromedioGF(), eqV->getPromedioGC());
    double lambdaV = calcularLambda(eqV->getPromedioGF(), eqL->getPromedioGC());

    // 4) Sacamos los goles (variable aleatoria Poisson)
    int golesL = muestrearPoisson(lambdaL);
    int golesV = muestrearPoisson(lambdaV);

    // 5) Si es eliminatoria y hay empate, prorroga y, si sigue, penales.
    if (etapa != FASE_GRUPOS && golesL == golesV) {
        // Prorroga: simulamos un mini-partido de 30 minutos con lambda/3
        int extraL = muestrearPoisson(lambdaL / 3.0);
        int extraV = muestrearPoisson(lambdaV / 3.0);
        golesL += extraL;
        golesV += extraV;
        estadisticas.setProrroga(true); // pone duracion=120

        // Si despues de prorroga sigue empatado -> penales
        if (golesL == golesV) {
            // Guardamos los goles antes de los penales, y luego decidimos
            estadisticas.setGoles(golesL, golesV);
            local->setGolesAnotados(golesL);
            visitante->setGolesAnotados(golesV);
            simularPenales();    // pone penalesL, penalesV
            // Repartimos goles entre jugadores (los normales/prorroga, no penales)
            repartirGoles(local, golesL);
            repartirGoles(visitante, golesV);
            yaSimulado = true;
            cerrarPartido();
            return;
        }
    }

    // Caso normal: guardamos resultado y repartimos goles
    estadisticas.setGoles(golesL, golesV);
    local->setGolesAnotados(golesL);
    visitante->setGolesAnotados(golesV);
    repartirGoles(local, golesL);
    repartirGoles(visitante, golesV);

    yaSimulado = true;
    cerrarPartido();
}

// Vuelca las estadisticas al final del partido:
//   - El equipo registra el resultado en su EstadisticasEquipo
//   - Cada jugador convocado actualiza su EstadisticasJugador (historico)
//   - El arbitro suma 1 partido dirigido
void Partido::cerrarPartido() {
    if (local != nullptr && visitante != nullptr) {
        Equipo* eqL = local->getEquipo();
        Equipo* eqV = visitante->getEquipo();
        if (eqL != nullptr && eqV != nullptr) {
            int gl = local->getGolesAnotados();
            int gv = visitante->getGolesAnotados();
            // En grupos cuenta el resultado normal.
            // En eliminatorias, si hubo penales el ganador "real" suma 3
            // pero la diferencia de goles oficial sigue siendo el empate.
            // Para mantenerlo simple, registramos el resultado tal cual.
            eqL->registrarResultado(gl, gv);
            eqV->registrarResultado(gv, gl);
        }
        local->volcarEstadisticasAcumuladas();
        visitante->volcarEstadisticasAcumuladas();
    }
    if (arbitro != nullptr) {
        arbitro->registrarPartidoDirigido();
    }
}

// Dos partidos son iguales si tienen el mismo id.
bool Partido::operator==(const Partido& otro) const {
    return id == otro.id;
}

ostream& operator<<(ostream& os, const Partido& p) {
    os << "[" << p.getNombreEtapa() << "] Partido #" << p.id;
    if (p.diaTorneo > 0) {
        os << " (dia " << p.diaTorneo << ")";
    }
    os << ": ";
    if (p.local != nullptr && p.local->getEquipo() != nullptr) {
        os << p.local->getEquipo()->getCodigoFIFA();
    } else {
        os << "???";
    }
    os << " vs ";
    if (p.visitante != nullptr && p.visitante->getEquipo() != nullptr) {
        os << p.visitante->getEquipo()->getCodigoFIFA();
    } else {
        os << "???";
    }
    if (p.yaSimulado) {
        os << " | " << p.estadisticas;
    } else {
        os << " | (no simulado)";
    }
    return os;
}

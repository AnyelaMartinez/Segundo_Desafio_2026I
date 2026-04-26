// ParticipacionEquipo.cpp
// Implementacion de la clase ParticipacionEquipo.

#include "ParticipacionEquipo.h"
#include "EstadisticasJugador.h"   // lo usamos en volcarEstadisticasAcumuladas

// Constructor por defecto: todo en cero.
ParticipacionEquipo::ParticipacionEquipo() {
    equipo = nullptr;
    partido = nullptr;
    esLocal = false;
    golesAnotados = 0;
}

// Constructor mas usado: nos pasan el equipo y el lado (local/visitante).
ParticipacionEquipo::ParticipacionEquipo(Equipo* eq, bool _esLocal) {
    equipo = eq;
    partido = nullptr;     // se asigna despues con setPartido()
    esLocal = _esLocal;
    golesAnotados = 0;
}

// Constructor copia: copia profunda de las estadisticas.
// Los punteros a Equipo, Partido y a Jugadores son SOLO REFERENCIAS:
// se copia el puntero, no el objeto al que apuntan.
ParticipacionEquipo::ParticipacionEquipo(const ParticipacionEquipo& otro) {
    equipo = otro.equipo;
    partido = otro.partido;
    esLocal = otro.esLocal;
    golesAnotados = otro.golesAnotados;

    // Copia de los punteros a Jugador (sin clonar al jugador)
    for (int i = 0; i < otro.convocados.getTamano(); i++) {
        convocados.agregar(otro.convocados[i]);
    }

    // Copia profunda de las estadisticas (estas si las creamos nosotros con new)
    for (int i = 0; i < otro.estadisticasJugadores.getTamano(); i++) {
        EstadisticasJugadorPartido* original = otro.estadisticasJugadores[i];
        if (original != nullptr) {
            estadisticasJugadores.agregar(
                new EstadisticasJugadorPartido(*original));
        } else {
            estadisticasJugadores.agregar(nullptr);
        }
    }
}

ParticipacionEquipo::~ParticipacionEquipo() {
    liberarEstadisticas();
    // OJO: NO borramos 'equipo', 'partido' ni los Jugador* convocados,
    // porque no son nuestros (son referencias prestadas).
}

// Borra solo las EstadisticasJugadorPartido (las que creamos con new aqui).
void ParticipacionEquipo::liberarEstadisticas() {
    for (int i = 0; i < estadisticasJugadores.getTamano(); i++) {
        if (estadisticasJugadores[i] != nullptr) {
            delete estadisticasJugadores[i];
        }
    }
    estadisticasJugadores.limpiar();
}

// Convoca a los primeros 11 jugadores ACTIVOS del equipo.
// Para cada uno crea una EstadisticasJugadorPartido nueva (con new).
void ParticipacionEquipo::convocarTitulares() {
    if (equipo == nullptr) return;

    int cuantos = 0;
    int total = equipo->getCantidadJugadores();
    for (int i = 0; i < total && cuantos < JUGADORES_CONVOCADOS; i++) {
        Jugador* j = equipo->getJugador(i);
        if (j != nullptr && j->estaActivo()) {
            convocados.agregar(j);
            estadisticasJugadores.agregar(new EstadisticasJugadorPartido());
            cuantos++;
        }
    }
    // Si no llegamos a 11 (porque hay muchos suspendidos), lo dejamos asi.
    // El profesor lo asume como caso raro.
}

// Convoca a un jugador especifico (le creamos su snapshot tambien)
void ParticipacionEquipo::convocarJugador(Jugador* j) {
    if (j != nullptr) {
        convocados.agregar(j);
        estadisticasJugadores.agregar(new EstadisticasJugadorPartido());
    }
}

// Registra un gol y lo atribuye al jugador en el indice dado.
void ParticipacionEquipo::registrarGolDeJugador(int indice) {
    golesAnotados++;
    if (indice >= 0 && indice < estadisticasJugadores.getTamano()) {
        EstadisticasJugadorPartido* e = estadisticasJugadores[indice];
        if (e != nullptr) {
            e->registrarGol();
        }
    }
}

void ParticipacionEquipo::registrarTarjetaDeJugador(int indice, bool esRoja) {
    if (indice < 0 || indice >= estadisticasJugadores.getTamano()) return;
    EstadisticasJugadorPartido* e = estadisticasJugadores[indice];
    if (e == nullptr) return;

    if (esRoja) {
        e->registrarRoja();
    } else {
        e->registrarAmarilla();
    }
}

void ParticipacionEquipo::registrarFaltaDeJugador(int indice) {
    if (indice < 0 || indice >= estadisticasJugadores.getTamano()) return;
    EstadisticasJugadorPartido* e = estadisticasJugadores[indice];
    if (e != nullptr) {
        e->registrarFalta();
    }
}

void ParticipacionEquipo::setEsLocal(bool b)        { esLocal = b; }
void ParticipacionEquipo::setPartido(Partido* p)    { partido = p; }
void ParticipacionEquipo::setGolesAnotados(int g)   { golesAnotados = g; }

Equipo* ParticipacionEquipo::getEquipo() const    { return equipo; }
Partido* ParticipacionEquipo::getPartido() const  { return partido; }
bool ParticipacionEquipo::getEsLocal() const      { return esLocal; }
int ParticipacionEquipo::getGolesAnotados() const { return golesAnotados; }
int ParticipacionEquipo::getCantidadConvocados() const {
    return convocados.getTamano();
}

Jugador* ParticipacionEquipo::getConvocado(int indice) const {
    if (indice < 0 || indice >= convocados.getTamano()) return nullptr;
    return convocados[indice];
}

EstadisticasJugadorPartido* ParticipacionEquipo::getEstadisticasJugador(int indice) const {
    if (indice < 0 || indice >= estadisticasJugadores.getTamano()) return nullptr;
    return estadisticasJugadores[indice];
}

// Al terminar el partido se llama a este metodo para que las estadisticas
// individuales de ESTE partido se sumen al historico de cada jugador.
void ParticipacionEquipo::volcarEstadisticasAcumuladas() {
    int n = convocados.getTamano();
    for (int i = 0; i < n; i++) {
        Jugador* j = convocados[i];
        EstadisticasJugadorPartido* sn = estadisticasJugadores[i];
        if (j == nullptr || sn == nullptr) continue;

        // Sumamos los datos del partido al historico del jugador
        j->registrarMinutos(sn->getMinutos());
        // golesEnMundial se actualiza si el partido es del Mundial.
        // Como esta clase solo se usa en partidos del Mundial, mandamos true.
        for (int g = 0; g < sn->getGoles(); g++) {
            j->registrarGol(true);
        }
        for (int t = 0; t < sn->getAmarillas(); t++) {
            j->registrarTarjeta(false);
        }
        if (sn->getRojas() > 0) {
            j->registrarTarjeta(true); // tambien lo deja inactivo
        }
        for (int f = 0; f < sn->getFaltas(); f++) {
            j->registrarFalta();
        }
        // Aumentamos partidos jugados del jugador
        if (j->estadisticas != nullptr) {
            j->estadisticas->aumentarPartidosJugados();
        }
    }
}

// Misma participacion = mismo equipo y mismo lado
bool ParticipacionEquipo::operator==(const ParticipacionEquipo& otro) const {
    if (equipo == nullptr || otro.equipo == nullptr) {
        return equipo == otro.equipo && esLocal == otro.esLocal;
    }
    return *equipo == *otro.equipo && esLocal == otro.esLocal;
}

ostream& operator<<(ostream& os, const ParticipacionEquipo& p) {
    string lado = p.esLocal ? "LOCAL" : "VISITANTE";
    if (p.equipo != nullptr) {
        os << "[" << lado << "] " << p.equipo->getCodigoFIFA()
           << " " << p.equipo->getNombre()
           << " - goles: " << p.golesAnotados
           << " - convocados: " << p.convocados.getTamano();
    } else {
        os << "[" << lado << "] (sin equipo)";
    }
    return os;
}

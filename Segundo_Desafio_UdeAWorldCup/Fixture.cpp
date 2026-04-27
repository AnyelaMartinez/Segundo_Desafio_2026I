// Fixture.cpp
// Implementacion de Fixture.

#include "Fixture.h"

Fixture::Fixture() {
    nombre = "Fixture";
    diaActualMaximo = 0;
    maxPartidosPorDia = 4;
    diasDescansoMinimo = 3;
}

Fixture::Fixture(string _nombre) {
    nombre = _nombre;
    diaActualMaximo = 0;
    maxPartidosPorDia = 4;
    diasDescansoMinimo = 3;
}

// Constructor copia: como los Partido* son prestados, basta con copiar
// los punteros (no hacemos copia profunda de los partidos).
Fixture::Fixture(const Fixture& otro) {
    nombre = otro.nombre;
    diaActualMaximo = otro.diaActualMaximo;
    maxPartidosPorDia = otro.maxPartidosPorDia;
    diasDescansoMinimo = otro.diasDescansoMinimo;
    for (int i = 0; i < otro.partidosAgendados.getTamano(); i++) {
        partidosAgendados.agregar(otro.partidosAgendados[i]);
    }
}

// Destructor: NO borramos los partidos, son prestados.
Fixture::~Fixture() {
    partidosAgendados.limpiar();
}

void Fixture::setNombre(string n) { nombre = n; }
string Fixture::getNombre() const { return nombre; }
int Fixture::getDiaActualMaximo() const { return diaActualMaximo; }
int Fixture::getCantidadPartidos() const { return partidosAgendados.getTamano(); }

void Fixture::setMaxPartidosPorDia(int m) {
    if (m > 0) maxPartidosPorDia = m;
}

void Fixture::setDiasDescansoMinimo(int d) {
    if (d >= 0) diasDescansoMinimo = d;
}

// Cuenta cuantos partidos hay en ese dia
int Fixture::contarPartidosEnDia(int dia) const {
    int cnt = 0;
    for (int i = 0; i < partidosAgendados.getTamano(); i++) {
        Partido* p = partidosAgendados[i];
        if (p != nullptr && p->getDia() == dia) {
            cnt++;
        }
    }
    return cnt;
}

// Revisa si el equipo eq ya juega ese dia
bool Fixture::equipoJuegaEseDia(Equipo* eq, int dia) const {
    if (eq == nullptr) return false;
    for (int i = 0; i < partidosAgendados.getTamano(); i++) {
        Partido* p = partidosAgendados[i];
        if (p == nullptr) continue;
        if (p->getDia() != dia) continue;
        // Comparamos por puntero porque cada equipo es unico en memoria
        if (p->getLocal()->getEquipo() == eq || p->getVisitante()->getEquipo() == eq) {
            return true;
        }
    }
    return false;
}

// Ultimo dia en que el equipo jugo
int Fixture::ultimoDiaDeEquipo(Equipo* eq) const {
    if (eq == nullptr) return 0;
    int ultimo = 0;
    for (int i = 0; i < partidosAgendados.getTamano(); i++) {
        Partido* p = partidosAgendados[i];
        if (p == nullptr) continue;
        if (p->getLocal()->getEquipo() == eq || p->getVisitante()->getEquipo() == eq) {
            if (p->getDia() > ultimo) {
                ultimo = p->getDia();
            }
        }
    }
    return ultimo;
}

// Logica principal: busca el primer dia disponible que cumpla:
//   1. No haya mas de maxPartidosPorDia partidos ya en ese dia.
//   2. Ningun equipo haya jugado en los ultimos diasDescansoMinimo dias.
//   3. Ningun equipo juegue ese mismo dia (obvio).
bool Fixture::agendarPartido(Partido* p) {
    return agendarPartidoDesde(p, 1);
}

bool Fixture::agendarPartidoDesde(Partido* p, int diaMinimo) {
    if (p == nullptr) return false;
    Equipo* local = p->getLocal()->getEquipo();
    Equipo* visitante = p->getVisitante()->getEquipo();
    if (local == nullptr || visitante == nullptr) return false;

    // El primer dia candidato debe ser:
    //  - >= diaMinimo (forzado por el llamador)
    //  - >= ultimo dia de cualquiera de los 2 equipos + descanso + 1
    int ultLocal = ultimoDiaDeEquipo(local);
    int ultVis   = ultimoDiaDeEquipo(visitante);
    int diaDesdeDescansoLocal = (ultLocal == 0) ? 1 : (ultLocal + diasDescansoMinimo + 1);
    int diaDesdeDescansoVis   = (ultVis == 0)   ? 1 : (ultVis + diasDescansoMinimo + 1);

    int dia = diaMinimo;
    if (diaDesdeDescansoLocal > dia) dia = diaDesdeDescansoLocal;
    if (diaDesdeDescansoVis > dia)   dia = diaDesdeDescansoVis;

    // Ahora vamos avanzando el dia hasta que tambien se cumpla
    // el limite de partidos por dia (raro que se llene, pero podria pasar).
    // Le ponemos un tope grande para no caer en bucle infinito si algo sale mal.
    int tope = dia + 365;
    while (dia <= tope) {
        if (contarPartidosEnDia(dia) < maxPartidosPorDia) {
            // Por seguridad: aunque ya respetamos el descanso, verificamos
            // que ningun equipo este jugando ese mismo dia (defensivo).
            if (!equipoJuegaEseDia(local, dia) &&
                !equipoJuegaEseDia(visitante, dia)) {
                p->setDia(dia);
                partidosAgendados.agregar(p);
                if (dia > diaActualMaximo) diaActualMaximo = dia;
                return true;
            }
        }
        dia++;
    }
    cout << "Advertencia: no se pudo agendar el partido " << p->getId() << endl;
    return false;
}

// Imprime un resumen dia por dia
void Fixture::mostrarCalendario() const {
    cout << "===== Calendario: " << nombre << " =====" << endl;
    cout << "Total partidos: " << partidosAgendados.getTamano()
         << " | Ultimo dia: " << diaActualMaximo << endl;

    for (int d = 1; d <= diaActualMaximo; d++) {
        int cnt = contarPartidosEnDia(d);
        if (cnt == 0) continue; // dias sin partidos no los listamos
        cout << "-- Dia " << d << " (" << cnt << " partidos) --" << endl;
        for (int i = 0; i < partidosAgendados.getTamano(); i++) {
            Partido* p = partidosAgendados[i];
            if (p == nullptr) continue;
            if (p->getDia() != d) continue;
            cout << "   " << *p << endl;
        }
    }
    cout << "==================================" << endl;
}

void Fixture::limpiar() {
    // No borramos los partidos, solo soltamos las referencias
    partidosAgendados.limpiar();
    diaActualMaximo = 0;
}

Fixture& Fixture::operator=(const Fixture& otro) {
    if (this == &otro) return *this;
    nombre = otro.nombre;
    diaActualMaximo = otro.diaActualMaximo;
    maxPartidosPorDia = otro.maxPartidosPorDia;
    diasDescansoMinimo = otro.diasDescansoMinimo;
    partidosAgendados.limpiar();
    for (int i = 0; i < otro.partidosAgendados.getTamano(); i++) {
        partidosAgendados.agregar(otro.partidosAgendados[i]);
    }
    return *this;
}

bool Fixture::operator==(const Fixture& otro) const {
    return nombre == otro.nombre;
}

ostream& operator<<(ostream& os, const Fixture& f) {
    os << "[Fixture " << f.nombre
       << " partidos=" << f.partidosAgendados.getTamano()
       << " ultimoDia=" << f.diaActualMaximo << "]";
    return os;
}

// Equipo.cpp
// Implementacion de la clase Equipo.

#include "Equipo.h"

// Constructor por defecto.
// Importante: dt arranca en nullptr (todavia no le hemos asignado uno).
// La lista de jugadores arranca vacia (lo hace la propia Lista<T>).
Equipo::Equipo() {
    nombre = "";
    codigoFIFA = "";
    rankingFIFA = 0;
    titulosMundiales = 0;
    promedioGolesFavor = 0.0;
    promedioGolesContra = 0.0;
    dt = nullptr;
}

// Constructor con parametros, mas comodo cuando lo cargamos desde el CSV.
Equipo::Equipo(string _nombre, string _codigoFIFA, int _rankingFIFA) {
    nombre = _nombre;
    codigoFIFA = _codigoFIFA;
    rankingFIFA = _rankingFIFA;
    titulosMundiales = 0;
    promedioGolesFavor = 0.0;
    promedioGolesContra = 0.0;
    dt = nullptr;
}

// Constructor copia: COPIA PROFUNDA.
// Tenemos memoria dinamica en dos sitios:
//   - dt        (un puntero a DirectorTecnico)
//   - jugadores (una Lista<Jugador*>, donde cada Jugador esta con new)
// Por eso necesitamos crear nuevos objetos con 'new', no solo copiar punteros.
Equipo::Equipo(const Equipo& otro) {
    nombre = otro.nombre;
    codigoFIFA = otro.codigoFIFA;
    rankingFIFA = otro.rankingFIFA;
    titulosMundiales = otro.titulosMundiales;
    promedioGolesFavor = otro.promedioGolesFavor;
    promedioGolesContra = otro.promedioGolesContra;

    federacion = otro.federacion;       // copia por valor
    confederacion = otro.confederacion; // copia por valor
    estadisticas = otro.estadisticas;   // copia por valor

    // Copia profunda del DT
    if (otro.dt != nullptr) {
        dt = new DirectorTecnico(*otro.dt);
    } else {
        dt = nullptr;
    }

    // Copia profunda de cada jugador
    for (int i = 0; i < otro.jugadores.getTamano(); i++) {
        Jugador* original = otro.jugadores[i];
        if (original != nullptr) {
            Jugador* copia = new Jugador(*original);
            jugadores.agregar(copia);
        }
    }
}

// Destructor: liberamos TODA la memoria reservada con 'new'.
Equipo::~Equipo() {
    // Borramos el DT si existe
    if (dt != nullptr) {
        delete dt;
        dt = nullptr;
    }
    // Borramos cada jugador antes de que la Lista se destruya por si sola.
    // La Lista<Jugador*> guarda PUNTEROS, ella sola no sabe que tiene que
    // hacer delete del objeto al que apuntan. Si no lo hicieramos, fuga
    // de memoria.
    for (int i = 0; i < jugadores.getTamano(); i++) {
        if (jugadores[i] != nullptr) {
            delete jugadores[i];
        }
    }
    jugadores.limpiar();
}

// --------- Setters ---------

void Equipo::setFederacion(const Federacion& f) {
    federacion = f;
}

void Equipo::setConfederacion(const ConfederacionContinental& c) {
    confederacion = c;
}

// Si ya habia un DT (raro, pero por si acaso), lo borramos antes de poner el nuevo.
void Equipo::setDirectorTecnico(DirectorTecnico* nuevoDT) {
    if (dt != nullptr) {
        delete dt;
    }
    dt = nuevoDT;
}

void Equipo::setPromedios(double gfa, double gcb) {
    promedioGolesFavor = gfa;
    promedioGolesContra = gcb;
}

void Equipo::setTitulosMundiales(int t) {
    titulosMundiales = t;
}

// El equipo "toma posesion" del puntero, asi que sera responsable de borrarlo
// en el destructor.
void Equipo::agregarJugador(Jugador* j) {
    if (j != nullptr) {
        jugadores.agregar(j);
    }
}

Jugador* Equipo::getJugador(int indice) const {
    if (indice < 0 || indice >= jugadores.getTamano()) {
        return nullptr;
    }
    return jugadores[indice];
}

int Equipo::getCantidadJugadores() const {
    return jugadores.getTamano();
}

int Equipo::contarJugadoresActivos() const {
    int cnt = 0;
    for (int i = 0; i < jugadores.getTamano(); i++) {
        if (jugadores[i] != nullptr && jugadores[i]->estaActivo()) {
            cnt++;
        }
    }
    return cnt;
}

// Reactiva a todos los jugadores. Lo llamamos al INICIO de un partido nuevo:
// la roja del partido anterior solo sanciona el siguiente.
void Equipo::reactivarJugadores() {
    for (int i = 0; i < jugadores.getTamano(); i++) {
        if (jugadores[i] != nullptr) {
            jugadores[i]->setActivo(true);
        }
    }
}

void Equipo::registrarResultado(int golesProp, int golesRiv) {
    estadisticas.registrarPartido(golesProp, golesRiv);
}

// --------- Getters ---------

string Equipo::getNombre() const     { return nombre; }
string Equipo::getCodigoFIFA() const { return codigoFIFA; }
int Equipo::getRanking() const       { return rankingFIFA; }
double Equipo::getPromedioGF() const { return promedioGolesFavor; }
double Equipo::getPromedioGC() const { return promedioGolesContra; }

EstadisticasEquipo& Equipo::getEstadisticas() {
    return estadisticas;
}

const EstadisticasEquipo& Equipo::getEstadisticas() const {
    return estadisticas;
}

void Equipo::mostrarPlantilla() const {
    cout << "Plantilla de " << codigoFIFA << " - " << nombre << ":" << endl;
    int n = jugadores.getTamano();
    if (n == 0) {
        cout << "  (sin jugadores cargados)" << endl;
        return;
    }
    // Mostramos solo los primeros 11 para no saturar la pantalla
    int max = (n < JUGADORES_CONVOCADOS) ? n : JUGADORES_CONVOCADOS;
    for (int i = 0; i < max; i++) {
        if (jugadores[i] != nullptr) {
            cout << "  " << *jugadores[i] << endl;
        }
    }
    if (n > JUGADORES_CONVOCADOS) {
        cout << "  ... (" << (n - JUGADORES_CONVOCADOS)
             << " jugadores mas en la convocatoria)" << endl;
    }
}

// Dos equipos son iguales si tienen el mismo codigo FIFA (es unico).
bool Equipo::operator==(const Equipo& otro) const {
    return codigoFIFA == otro.codigoFIFA;
}

// Equipo a < Equipo b  significa  "a esta por debajo en la tabla que b".
// Reusamos la logica que ya esta en EstadisticasEquipo.
bool Equipo::operator<(const Equipo& otro) const {
    return this->estadisticas < otro.estadisticas;
}

// Imprime: "ARG Argentina (CONMEBOL) | Pts=... DG=..."
ostream& operator<<(ostream& os, const Equipo& e) {
    os << e.codigoFIFA << " " << e.nombre
       << " (" << e.confederacion.sigla << ")"
       << " | " << e.estadisticas;
    return os;
}

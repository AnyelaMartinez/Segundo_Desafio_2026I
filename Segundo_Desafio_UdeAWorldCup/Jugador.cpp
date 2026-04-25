// Jugador.cpp
// Implementacion de la clase Jugador.

#include "Jugador.h"

// Constructor por defecto: reserva memoria para las estadisticas.
Jugador::Jugador() {
    id = 0;
    nombre = "";
    apellido = "";
    numeroCamiseta = 0;
    activo = true;
    estadisticas = new EstadisticasJugador();  // <-- memoria dinamica
}

// Constructor con parametros (es el que mas usaremos al cargar desde CSV)
Jugador::Jugador(int _id, string _nombre, string _apellido, int _dorsal) {
    id = _id;
    nombre = _nombre;
    apellido = _apellido;
    numeroCamiseta = _dorsal;
    activo = true;
    estadisticas = new EstadisticasJugador();
}

// Constructor copia: COPIA PROFUNDA.
// Si solo copiaramos el puntero, dos jugadores compartirian las mismas
// estadisticas, y el destructor borraria la misma memoria dos veces
// (double free -> crash).
Jugador::Jugador(const Jugador& otro) {
    id = otro.id;
    nombre = otro.nombre;
    apellido = otro.apellido;
    numeroCamiseta = otro.numeroCamiseta;
    activo = otro.activo;
    // Creamos una copia nueva de las estadisticas usando su copy constructor
    estadisticas = new EstadisticasJugador(*otro.estadisticas);
}

// Destructor: libera la memoria reservada con 'new'.
Jugador::~Jugador() {
    delete estadisticas;
    estadisticas = nullptr; // buena practica para evitar dangling pointer
}

// Getters
int Jugador::getId() const              { return id; }
string Jugador::getNombre() const       { return nombre; }
string Jugador::getApellido() const     { return apellido; }
int Jugador::getDorsal() const          { return numeroCamiseta; }
bool Jugador::estaActivo() const        { return activo; }

void Jugador::setActivo(bool a) {
    activo = a;
}

// Llama a los metodos de EstadisticasJugador por medio del puntero.
// El if (estadisticas != nullptr) es defensivo.
void Jugador::registrarMinutos(int min) {
    if (estadisticas != nullptr) {
        estadisticas->actualizarMinutos(min);
    }
}

void Jugador::registrarGol(bool esMundial) {
    if (estadisticas != nullptr) {
        estadisticas->actualizarGol(esMundial);
    }
}

void Jugador::registrarTarjeta(bool esRoja) {
    if (estadisticas != nullptr) {
        if (esRoja) {
            estadisticas->actualizarTarjetaRoja();
            fueExpulsado();  // si fue roja, queda inactivo
        } else {
            estadisticas->actualizarTarjetaAmarilla();
        }
    }
}

void Jugador::registrarFalta() {
    if (estadisticas != nullptr) {
        estadisticas->actualizarFalta();
    }
}

// Cuando un jugador recibe roja, lo dejamos inactivo.
// Asi el DT sabe que no puede convocarlo en el proximo partido.
void Jugador::fueExpulsado() {
    activo = false;
}

// Dos jugadores son iguales si tienen el mismo id.
bool Jugador::operator==(const Jugador& otro) const {
    return id == otro.id;
}

// Para ordenar jugadores por goles en el Mundial (goleadores).
// Queremos que el que MAS goles tenga quede primero, asi que
// "a < b" se interpreta como "a tiene MENOS goles que b".
bool Jugador::operator<(const Jugador& otro) const {
    // ojo: accedemos con -> porque estadisticas es puntero
    return this->estadisticas->getGolesEnMundial()
         < otro.estadisticas->getGolesEnMundial();
}

// Impresion amigable: "#10 Messi (A.) - goles mundial: 2"
ostream& operator<<(ostream& os, const Jugador& j) {
    os << "#" << j.numeroCamiseta << " "
       << j.nombre << " " << j.apellido;
    if (!j.activo) {
        os << " [INACTIVO]";
    }
    if (j.estadisticas != nullptr) {
        os << " | goles mundial: " << j.estadisticas->getGolesEnMundial();
    }
    return os;
}

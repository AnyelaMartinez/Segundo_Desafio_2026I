// Grupo.cpp
// Implementacion de la clase Grupo.

#include "Grupo.h"

Grupo::Grupo() {
    letra = '?';
}

Grupo::Grupo(char _letra) {
    letra = _letra;
}

Grupo::Grupo(const Grupo& otro) {
    letra = otro.letra;
    // Copiamos los punteros a equipos
    for (int i = 0; i < otro.equipos.getTamano(); i++) {
        equipos.agregar(otro.equipos[i]);
    }
    // Copiamos los punteros a partidos
    for (int i = 0; i < otro.partidos.getTamano(); i++) {
        partidos.agregar(otro.partidos[i]);
    }
    // La tabla la copiamos por valor (PosicionGrupo no tiene memoria propia)
    for (int i = 0; i < otro.tablaPosiciones.getTamano(); i++) {
        tablaPosiciones.agregar(otro.tablaPosiciones[i]);
    }
}

Grupo::~Grupo() {
    // No borramos equipos ni partidos: son referencias prestadas.
}

void Grupo::setLetra(char l) { letra = l; }
char Grupo::getLetra() const { return letra; }

void Grupo::agregarEquipo(Equipo* e) {
    if (e != nullptr) {
        equipos.agregar(e);
    }
}

void Grupo::agregarPartido(Partido* p) {
    if (p != nullptr) {
        partidos.agregar(p);
    }
}

int Grupo::getCantidadEquipos() const   { return equipos.getTamano(); }
int Grupo::getCantidadPartidos() const  { return partidos.getTamano(); }

// Construye la tabla y la ordena de mejor a peor.
// Como nuestra Lista<T> no tiene 'sort', hacemos un bubble sort manual.
// Es lento pero como solo son 4 equipos no importa.
void Grupo::calcularTabla() {
    // Limpiamos la tabla anterior
    tablaPosiciones.limpiar();

    // Llenamos la tabla con los equipos del grupo
    for (int i = 0; i < equipos.getTamano(); i++) {
        PosicionGrupo pos(equipos[i], 0);
        tablaPosiciones.agregar(pos);
    }

    // Bubble sort: para 4 elementos es perfecto
    int n = tablaPosiciones.getTamano();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            // Queremos al MEJOR primero, asi que si el de la izquierda
            // es PEOR (operator<) lo intercambiamos.
            if (tablaPosiciones[j] < tablaPosiciones[j + 1]) {
                PosicionGrupo tmp = tablaPosiciones[j];
                tablaPosiciones[j] = tablaPosiciones[j + 1];
                tablaPosiciones[j + 1] = tmp;
            }
        }
    }

    // Asignamos la posicion final (1..4)
    for (int i = 0; i < n; i++) {
        tablaPosiciones[i].setPosicion(i + 1);
        tablaPosiciones[i].setClasificado(false); // por defecto nadie clasifica
    }
}

void Grupo::marcarClasificados(int n) {
    int total = tablaPosiciones.getTamano();
    if (n > total) n = total;
    for (int i = 0; i < n; i++) {
        tablaPosiciones[i].setClasificado(true);
    }
}

Equipo* Grupo::getEquipoEnPosicion(int posicion) const {
    int idx = posicion - 1;
    if (idx < 0 || idx >= tablaPosiciones.getTamano()) return nullptr;
    return tablaPosiciones[idx].getEquipo();
}

int Grupo::getCantidadClasificados() const {
    int cnt = 0;
    for (int i = 0; i < tablaPosiciones.getTamano(); i++) {
        if (tablaPosiciones[i].estaClasificado()) cnt++;
    }
    return cnt;
}

void Grupo::mostrarTabla() const {
    cout << "Grupo " << letra << ":" << endl;
    cout << "------------------------------------------------------" << endl;
    if (tablaPosiciones.estaVacia()) {
        cout << "  (la tabla no se ha calculado todavia)" << endl;
        // Mostramos solo los equipos
        for (int i = 0; i < equipos.getTamano(); i++) {
            if (equipos[i] != nullptr) {
                cout << "  - " << *equipos[i] << endl;
            }
        }
        return;
    }
    for (int i = 0; i < tablaPosiciones.getTamano(); i++) {
        cout << "  " << tablaPosiciones[i] << endl;
    }
}

bool Grupo::operator==(const Grupo& otro) const {
    return letra == otro.letra;
}

ostream& operator<<(ostream& os, const Grupo& g) {
    os << "Grupo " << g.letra << " ("
       << g.equipos.getTamano() << " equipos, "
       << g.partidos.getTamano() << " partidos)";
    return os;
}

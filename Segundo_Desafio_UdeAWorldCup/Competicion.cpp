// Competicion.cpp
// Implementacion - Parte A: estructura, sedes, arbitros, bombos y sorteo.
// Implementacion - Parte B: fase de grupos (partidos, simulacion, tablas).

#include "Competicion.h"
#include "Jugador.h"
#include <cstdlib>   // rand

Competicion::Competicion() {
    nombre = "FIFA World Cup";
    anio = 2026;
    sorteoRealizado = false;
    partidosGrupoGenerados = false;
    gruposSimulados = false;
    eliminatoriasGeneradas = false;
    eliminatoriasSimuladas = false;
    campeon = nullptr;
    subcampeon = nullptr;
    tercero = nullptr;
    cuarto = nullptr;
    fixture.setNombre("Fixture Mundial");
    // Numeramos los 4 bombos (1..4) para que los reportes sean legibles
    bombos[0].setNumero(1);
    bombos[1].setNumero(2);
    bombos[2].setNumero(3);
    bombos[3].setNumero(4);
}

Competicion::Competicion(string _nombre, int _anio) {
    nombre = _nombre;
    anio = _anio;
    sorteoRealizado = false;
    partidosGrupoGenerados = false;
    gruposSimulados = false;
    eliminatoriasGeneradas = false;
    eliminatoriasSimuladas = false;
    campeon = nullptr;
    subcampeon = nullptr;
    tercero = nullptr;
    cuarto = nullptr;
    fixture.setNombre("Fixture " + _nombre);
    bombos[0].setNumero(1);
    bombos[1].setNumero(2);
    bombos[2].setNumero(3);
    bombos[3].setNumero(4);
}

// "Copia ligera": copiamos los datos basicos pero NO los grupos/sedes/etc.
// Para no liarnos con doble delete. Casi nunca necesitamos copiar una
// Competicion, pero la regla de los 3 lo pide.
Competicion::Competicion(const Competicion& otro) {
    nombre = otro.nombre;
    anio = otro.anio;
    sorteoRealizado = false; // la copia empieza vacia
    for (int i = 0; i < otro.equipos.getTamano(); i++) {
        equipos.agregar(otro.equipos[i]); // prestados, igual
    }
    for (int i = 0; i < 4; i++) {
        bombos[i] = otro.bombos[i];
    }
    fixture.setNombre(otro.fixture.getNombre());
}

// Libera todo lo PROPIO (grupos, sedes, arbitros, partidos).
// Los equipos NO se borran porque son prestados.
Competicion::~Competicion() {
    limpiar();
}

void Competicion::limpiar() {
    // Borrar grupos
    for (int i = 0; i < grupos.getTamano(); i++) {
        if (grupos[i] != nullptr) delete grupos[i];
    }
    grupos.limpiar();
    // Borrar sedes
    for (int i = 0; i < sedes.getTamano(); i++) {
        if (sedes[i] != nullptr) delete sedes[i];
    }
    sedes.limpiar();
    // Borrar arbitros
    for (int i = 0; i < arbitros.getTamano(); i++) {
        if (arbitros[i] != nullptr) delete arbitros[i];
    }
    arbitros.limpiar();
    // Borrar partidos
    for (int i = 0; i < partidos.getTamano(); i++) {
        if (partidos[i] != nullptr) delete partidos[i];
    }
    partidos.limpiar();

    fixture.limpiar();
    sorteoRealizado = false;
    partidosGrupoGenerados = false;
    gruposSimulados = false;
    eliminatoriasGeneradas = false;
    eliminatoriasSimuladas = false;
    clasificadosR16.limpiar();
    campeon = nullptr;
    subcampeon = nullptr;
    tercero = nullptr;
    cuarto = nullptr;
}

void Competicion::setEquipos(const Lista<Equipo*>& fuente) {
    equipos.limpiar();
    for (int i = 0; i < fuente.getTamano(); i++) {
        equipos.agregar(fuente[i]);
    }
}

// 16 sedes del Mundial 2026 (USA, Mexico, Canada).
// Hardcodeadas porque no es prioridad parsearlas de otro CSV.
void Competicion::cargarSedes() {
    sedes.agregar(new Sede("MetLife Stadium", "East Rutherford", "USA", 82500));
    sedes.agregar(new Sede("SoFi Stadium", "Inglewood", "USA", 70240));
    sedes.agregar(new Sede("AT&T Stadium", "Arlington", "USA", 80000));
    sedes.agregar(new Sede("Mercedes-Benz Stadium", "Atlanta", "USA", 71000));
    sedes.agregar(new Sede("Hard Rock Stadium", "Miami Gardens", "USA", 65326));
    sedes.agregar(new Sede("NRG Stadium", "Houston", "USA", 72220));
    sedes.agregar(new Sede("Arrowhead Stadium", "Kansas City", "USA", 76416));
    sedes.agregar(new Sede("GEHA Field", "Kansas City", "USA", 76416));
    sedes.agregar(new Sede("Levi's Stadium", "Santa Clara", "USA", 68500));
    sedes.agregar(new Sede("Lincoln Financial Field", "Filadelfia", "USA", 67594));
    sedes.agregar(new Sede("Lumen Field", "Seattle", "USA", 68740));
    sedes.agregar(new Sede("Gillette Stadium", "Foxborough", "USA", 65878));
    sedes.agregar(new Sede("Estadio Azteca", "Ciudad de Mexico", "Mexico", 87000));
    sedes.agregar(new Sede("Estadio Akron", "Guadalajara", "Mexico", 49850));
    sedes.agregar(new Sede("Estadio BBVA", "Monterrey", "Mexico", 53500));
    sedes.agregar(new Sede("BMO Field", "Toronto", "Canada", 30000));
    cout << "Sedes cargadas: " << sedes.getTamano() << endl;
}

// Hardcodeamos un grupo razonable de arbitros (12, uno por cada grupo).
void Competicion::cargarArbitros() {
    arbitros.agregar(new Arbitro("Wilton Sampaio", "Brasil"));
    arbitros.agregar(new Arbitro("Facundo Tello", "Argentina"));
    arbitros.agregar(new Arbitro("Anthony Taylor", "Inglaterra"));
    arbitros.agregar(new Arbitro("Daniele Orsato", "Italia"));
    arbitros.agregar(new Arbitro("Daniel Siebert", "Alemania"));
    arbitros.agregar(new Arbitro("Clement Turpin", "Francia"));
    arbitros.agregar(new Arbitro("Cesar Ramos", "Mexico"));
    arbitros.agregar(new Arbitro("Ismail Elfath", "USA"));
    arbitros.agregar(new Arbitro("Mustapha Ghorbal", "Argelia"));
    arbitros.agregar(new Arbitro("Mohammed Al Hoaish", "Arabia Saudita"));
    arbitros.agregar(new Arbitro("Ko Hyung-Jin", "Corea del Sur"));
    arbitros.agregar(new Arbitro("Said Martinez", "Honduras"));
    cout << "Arbitros cargados: " << arbitros.getTamano() << endl;
}

// Reparte los 48 equipos en 4 bombos por ranking FIFA:
//   Bombo 1: ranking 1..12
//   Bombo 2: ranking 13..24
//   Bombo 3: ranking 25..36
//   Bombo 4: ranking 37..48
// (Es la convencion que usamos para el desafio: separar por ranking.)
void Competicion::cargarBombos() {
    // Limpiar bombos antes de llenar (por si llaman dos veces).
    // El campo 'equipos' del Bombo es publico y son punteros prestados,
    // asi que basta con limpiarlos sin borrar los Equipo.
    for (int i = 0; i < 4; i++) {
        bombos[i].equipos.limpiar();
    }

    if (equipos.getTamano() != TOTAL_EQUIPOS) {
        cout << "Advertencia: se esperaban " << TOTAL_EQUIPOS
             << " equipos y hay " << equipos.getTamano() << "." << endl;
    }

    for (int i = 0; i < equipos.getTamano(); i++) {
        Equipo* e = equipos[i];
        if (e == nullptr) continue;
        int rk = e->getRankingFIFA();
        // Decidimos a que bombo va segun el ranking
        if (rk >= 1 && rk <= 12) {
            bombos[0].agregarEquipo(e);
        } else if (rk >= 13 && rk <= 24) {
            bombos[1].agregarEquipo(e);
        } else if (rk >= 25 && rk <= 36) {
            bombos[2].agregarEquipo(e);
        } else {
            // Resto van al bombo 4 (ranking 37..48 o cualquier otro)
            bombos[3].agregarEquipo(e);
        }
    }

    cout << "Bombos cargados:" << endl;
    for (int i = 0; i < 4; i++) {
        cout << "  Bombo " << bombos[i].getNumero()
             << " -> " << bombos[i].getCantidad() << " equipos" << endl;
    }
}

// Heuristica: este equipo entra en este grupo?
// Reglas:
//  - El grupo no puede tener mas de 4 equipos.
//  - No puede haber 2 equipos de la misma confederacion en el grupo
//    (salvo UEFA, donde se permiten 2).
bool Competicion::grupoAceptaEquipo(Grupo* g, Equipo* eq) const {
    if (g == nullptr || eq == nullptr) return false;
    if (g->getCantidadEquipos() >= EQUIPOS_POR_GRUPO) return false;

    string sigla = eq->getConfederacion().sigla;
    // Cuantos equipos del grupo ya son de esa confederacion
    int cnt = 0;
    for (int i = 0; i < g->getCantidadEquipos(); i++) {
        // El campo 'equipos' del Grupo es publico (Lista<Equipo*>)
        Equipo* x = g->equipos[i];
        if (x == nullptr) continue;
        if (x->getConfederacion().sigla == sigla) cnt++;
    }

    // UEFA permite hasta 2 por grupo, las otras solo 1.
    if (sigla == "UEFA") {
        return cnt < 2;
    }
    return cnt < 1;
}

// Toma un equipo aleatorio del bombo y lo intenta meter en algun grupo
// que lo acepte. Si encuentra, lo agrega y devuelve true.
bool Competicion::asignarDesdeBombo(Bombo& b) {
    // Vamos a intentar varias veces. Si despues de muchos intentos no
    // se puede, devolvemos false (caso muy improbable con 12 grupos).
    int intentosMaximos = b.getCantidad() * 10 + 50;

    for (int intento = 0; intento < intentosMaximos; intento++) {
        if (b.getCantidad() == 0) return false;

        // Sacamos un equipo aleatorio del bombo
        Equipo* eq = b.sacarEquipoAlAzar();
        if (eq == nullptr) return false;

        // Buscamos un grupo donde quepa
        for (int g = 0; g < grupos.getTamano(); g++) {
            Grupo* grp = grupos[g];
            if (grupoAceptaEquipo(grp, eq)) {
                grp->agregarEquipo(eq);
                return true;
            }
        }

        // No encontramos grupo: lo devolvemos al bombo y reintentamos.
        // (raro, pero puede pasar al final del sorteo si los pocos
        // grupos restantes ya tienen confederaciones repetidas).
        b.agregarEquipo(eq);
    }

    cout << "Advertencia: no pude colocar equipo del " << b.getNombre() << endl;
    return false;
}

// Realiza el sorteo: crea 12 grupos vacios y rellena uno desde cada bombo.
bool Competicion::realizarSorteo() {
    if (equipos.getTamano() < TOTAL_EQUIPOS) {
        cout << "No hay 48 equipos cargados." << endl;
        return false;
    }
    // Si ya habia un sorteo previo, limpiamos los grupos
    for (int i = 0; i < grupos.getTamano(); i++) {
        if (grupos[i] != nullptr) delete grupos[i];
    }
    grupos.limpiar();

    // Crear 12 grupos con letras 'A'..'L'
    for (int i = 0; i < TOTAL_GRUPOS; i++) {
        char letra = (char)('A' + i);
        Grupo* g = new Grupo(letra);
        grupos.agregar(g);
    }

    // Para cada bombo (1..4), asignar un equipo a cada grupo (A..L).
    // Es decir: 12 extracciones por bombo.
    for (int b = 0; b < 4; b++) {
        for (int k = 0; k < TOTAL_GRUPOS; k++) {
            bool ok = asignarDesdeBombo(bombos[b]);
            if (!ok) {
                cout << "Error en sorteo: bombo " << (b+1)
                     << " no pudo asignar." << endl;
                return false;
            }
        }
    }

    sorteoRealizado = true;
    cout << "Sorteo realizado con exito. " << TOTAL_GRUPOS
         << " grupos armados." << endl;
    return true;
}

int Competicion::getCantidadEquipos() const { return equipos.getTamano(); }
int Competicion::getCantidadGrupos() const { return grupos.getTamano(); }
bool Competicion::getSorteoRealizado() const { return sorteoRealizado; }

Grupo* Competicion::getGrupo(int indice) {
    if (indice < 0 || indice >= grupos.getTamano()) return nullptr;
    return grupos[indice];
}

Grupo* Competicion::getGrupoPorLetra(char letra) {
    for (int i = 0; i < grupos.getTamano(); i++) {
        if (grupos[i] != nullptr && grupos[i]->getLetra() == letra) {
            return grupos[i];
        }
    }
    return nullptr;
}

void Competicion::mostrarBombos() const {
    cout << "===== Bombos =====" << endl;
    for (int i = 0; i < 4; i++) {
        cout << bombos[i] << endl;
        for (int j = 0; j < bombos[i].getCantidad(); j++) {
            Equipo* e = bombos[i].getEquipo(j);
            if (e != nullptr) {
                cout << "   " << e->getCodigoFIFA()
                     << " - " << e->getNombre()
                     << " (rk " << e->getRankingFIFA() << ")"
                     << endl;
            }
        }
    }
    cout << "==================" << endl;
}

void Competicion::mostrarGrupos() const {
    cout << "===== Grupos del Mundial =====" << endl;
    for (int i = 0; i < grupos.getTamano(); i++) {
        Grupo* g = grupos[i];
        if (g == nullptr) continue;
        cout << "Grupo " << g->getLetra()
             << " (" << g->getCantidadEquipos() << " equipos):" << endl;
        for (int j = 0; j < g->getCantidadEquipos(); j++) {
            Equipo* e = g->equipos[j];
            if (e != nullptr) {
                cout << "   " << e->getCodigoFIFA()
                     << " - " << e->getNombre()
                     << "  [" << e->getConfederacion().sigla << "]"
                     << "  (rk " << e->getRankingFIFA() << ")"
                     << endl;
            }
        }
    }
    cout << "===============================" << endl;
}

Competicion& Competicion::operator=(const Competicion& otro) {
    if (this == &otro) return *this;
    limpiar();
    nombre = otro.nombre;
    anio = otro.anio;
    sorteoRealizado = false;
    equipos.limpiar();
    for (int i = 0; i < otro.equipos.getTamano(); i++) {
        equipos.agregar(otro.equipos[i]);
    }
    for (int i = 0; i < 4; i++) {
        bombos[i] = otro.bombos[i];
    }
    return *this;
}

bool Competicion::operator==(const Competicion& otro) const {
    return nombre == otro.nombre && anio == otro.anio;
}

ostream& operator<<(ostream& os, const Competicion& c) {
    os << "[Competicion " << c.nombre << " " << c.anio
       << " equipos=" << c.equipos.getTamano()
       << " grupos=" << c.grupos.getTamano()
       << " sorteo=" << (c.sorteoRealizado ? "si" : "no")
       << "]";
    return os;
}

// =========================================================
// =================== PARTE B: GRUPOS =====================
// =========================================================

// Genera 11 jugadores ficticios para cada equipo que aun no tenga.
// Lo hacemos asi porque el CSV de selecciones NO trae jugadores;
// pero sin jugadores no se puede simular el partido.
void Competicion::asegurarJugadoresEnEquipos() {
    int idGlobal = 1; // contador para que cada jugador tenga id unico
    for (int i = 0; i < equipos.getTamano(); i++) {
        Equipo* eq = equipos[i];
        if (eq == nullptr) continue;

        // Si ya tiene jugadores, no le agregamos mas
        if (eq->getCantidadJugadores() >= JUGADORES_CONVOCADOS) continue;

        // Le agregamos los que falten hasta llegar a 11
        int faltan = JUGADORES_CONVOCADOS - eq->getCantidadJugadores();
        for (int k = 0; k < faltan; k++) {
            // Numero de camiseta entre 1 y 23
            int dorsal = (k % 23) + 1;
            string nombre = "Jug" + eq->getCodigoFIFA();
            string apellido = "Num" + std::to_string(dorsal);
            Jugador* j = new Jugador(idGlobal, nombre, apellido, dorsal);
            eq->agregarJugador(j);
            idGlobal++;
        }
    }
}

// Para cada grupo (12) genera los 6 partidos round-robin (cada equipo
// juega contra los otros 3). En total 72 partidos de fase de grupos.
// A cada partido le asigna sede y arbitro al azar y lo agenda en el fixture.
void Competicion::generarPartidosFaseGrupos() {
    if (!sorteoRealizado) {
        cout << "Primero hay que hacer el sorteo." << endl;
        return;
    }
    if (partidosGrupoGenerados) {
        cout << "Los partidos de grupos ya se habian generado." << endl;
        return;
    }
    if (sedes.getTamano() == 0 || arbitros.getTamano() == 0) {
        cout << "Faltan sedes o arbitros." << endl;
        return;
    }

    asegurarJugadoresEnEquipos();

    int idPartido = 1;
    for (int g = 0; g < grupos.getTamano(); g++) {
        Grupo* grp = grupos[g];
        if (grp == nullptr) continue;

        int n = grp->getCantidadEquipos();
        // Round-robin: para cada par (i, j) con i<j hay un partido.
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                Equipo* eqL = grp->equipos[i];
                Equipo* eqV = grp->equipos[j];
                if (eqL == nullptr || eqV == nullptr) continue;

                Partido* p = new Partido(idPartido, FASE_GRUPOS, eqL, eqV);
                idPartido++;

                // Sede aleatoria
                int idxSede = rand() % sedes.getTamano();
                p->setSede(sedes[idxSede]);

                // Arbitro aleatorio
                int idxArb = rand() % arbitros.getTamano();
                p->setArbitro(arbitros[idxArb]);

                // Agendar en el calendario (respeta 4/dia y 3 dias descanso)
                fixture.agendarPartido(p);

                // Anotar en el grupo y en la lista global
                grp->agregarPartido(p);
                partidos.agregar(p);
            }
        }
    }

    partidosGrupoGenerados = true;
    cout << "Partidos de fase de grupos generados: "
         << partidos.getTamano() << endl;
    cout << "Ultimo dia agendado: " << fixture.getDiaActualMaximo() << endl;
}

// Recorre TODOS los partidos en orden por dia y los simula.
// Como Partido::simular() ya actualiza estadisticas de los equipos
// y de los jugadores, no hace falta tocar nada mas aqui.
void Competicion::simularFaseGrupos() {
    if (!partidosGrupoGenerados) {
        cout << "Primero hay que generar los partidos." << endl;
        return;
    }
    if (gruposSimulados) {
        cout << "Los grupos ya estaban simulados." << endl;
        return;
    }

    int simulados = 0;
    // Recorremos por dias para que sea cronologico
    for (int dia = 1; dia <= fixture.getDiaActualMaximo(); dia++) {
        for (int i = 0; i < partidos.getTamano(); i++) {
            Partido* p = partidos[i];
            if (p == nullptr) continue;
            if (p->getEtapa() != FASE_GRUPOS) continue;
            if (p->getDia() != dia) continue;
            // Solo si todavia no se ha simulado
            if (p->yaSimulado) continue;

            p->simular();
            simulados++;
        }
    }

    gruposSimulados = true;
    cout << "Partidos simulados en fase de grupos: " << simulados << endl;
}

// Pide a cada grupo que calcule su tabla y marque los 2 primeros
// como clasificados a R16.
void Competicion::construirTablasYClasificar() {
    if (!gruposSimulados) {
        cout << "Primero hay que simular la fase de grupos." << endl;
        return;
    }
    for (int i = 0; i < grupos.getTamano(); i++) {
        Grupo* g = grupos[i];
        if (g == nullptr) continue;
        g->calcularTabla();
        g->marcarClasificados(EQUIPOS_CLASIFICAN_POR_GRUPO);
    }
    cout << "Tablas construidas y clasificados marcados (2 por grupo)." << endl;
}

// Imprime los resultados de fase de grupos, dia por dia.
void Competicion::mostrarResultadosFaseGrupos() const {
    cout << "===== Resultados Fase de Grupos =====" << endl;
    for (int dia = 1; dia <= fixture.getDiaActualMaximo(); dia++) {
        bool hayPartidoEseDia = false;
        for (int i = 0; i < partidos.getTamano(); i++) {
            Partido* p = partidos[i];
            if (p == nullptr) continue;
            if (p->getEtapa() != FASE_GRUPOS) continue;
            if (p->getDia() != dia) continue;
            if (!hayPartidoEseDia) {
                cout << "-- Dia " << dia << " --" << endl;
                hayPartidoEseDia = true;
            }
            cout << "   " << *p << endl;
        }
    }
    cout << "=====================================" << endl;
}

// Imprime las 12 tablas (una por grupo).
void Competicion::mostrarTablasGrupos() const {
    cout << "===== Tablas por Grupo =====" << endl;
    for (int i = 0; i < grupos.getTamano(); i++) {
        Grupo* g = grupos[i];
        if (g == nullptr) continue;
        g->mostrarTabla();
    }
    cout << "============================" << endl;
}

// =========================================================
// ================ PARTE C: ELIMINATORIAS =================
// =========================================================

// Selecciona los 32 equipos que pasan a R16:
//   - 1ros y 2dos de cada uno de los 12 grupos = 24 equipos
//   - los 8 mejores 3ros = 8 equipos
// Total = 32.
bool Competicion::seleccionarClasificadosR16() {
    if (!gruposSimulados) {
        cout << "Primero hay que simular fase de grupos." << endl;
        return false;
    }

    clasificadosR16.limpiar();

    // 1) Agarrar los 1ros y 2dos de cada grupo
    for (int i = 0; i < grupos.getTamano(); i++) {
        Grupo* g = grupos[i];
        if (g == nullptr) continue;
        Equipo* primero = g->getEquipoEnPosicion(1);
        Equipo* segundo = g->getEquipoEnPosicion(2);
        if (primero != nullptr) clasificadosR16.agregar(primero);
        if (segundo != nullptr) clasificadosR16.agregar(segundo);
    }

    // 2) Mejores 8 terceros: tomamos los 12 terceros y los ordenamos
    //    por puntos -> diferencia de gol -> goles a favor (bubble sort).
    Lista<Equipo*> terceros;
    for (int i = 0; i < grupos.getTamano(); i++) {
        Grupo* g = grupos[i];
        if (g == nullptr) continue;
        Equipo* tercero = g->getEquipoEnPosicion(3);
        if (tercero != nullptr) terceros.agregar(tercero);
    }

    // Bubble sort: mejor primero
    int n = terceros.getTamano();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            Equipo* a = terceros[j];
            Equipo* b = terceros[j + 1];
            if (a == nullptr || b == nullptr) continue;
            if (b->getEstadisticas().vaPorEncimaDe(a->getEstadisticas())) {
                terceros[j] = b;
                terceros[j + 1] = a;
            }
        }
    }

    // Tomamos los primeros 8 (o menos si por algun motivo no hay 12 grupos)
    int aTomar = (terceros.getTamano() < 8) ? terceros.getTamano() : 8;
    for (int i = 0; i < aTomar; i++) {
        clasificadosR16.agregar(terceros[i]);
    }

    cout << "Clasificados a R16: " << clasificadosR16.getTamano()
         << " equipos" << endl;
    return clasificadosR16.getTamano() == 32;
}

// Crea los 16 partidos de R16 emparejando los 32 clasificados de a pares.
// Los emparejamos en orden simple: clasif[0] vs clasif[1], clasif[2] vs clasif[3], ...
void Competicion::generarPartidosR16() {
    if (clasificadosR16.getTamano() < 32) {
        cout << "No hay 32 clasificados aun." << endl;
        return;
    }
    if (sedes.getTamano() == 0 || arbitros.getTamano() == 0) {
        cout << "Faltan sedes o arbitros." << endl;
        return;
    }

    // Los partidos de R16 deben empezar al menos 1 dia despues
    // del ultimo dia de fase de grupos.
    int diaMinimo = fixture.getDiaActualMaximo() + 1;

    // Antes de simular, reactivamos jugadores suspendidos por roja
    // (el desafio dice que la roja saca al jugador para "el proximo partido").
    for (int i = 0; i < equipos.getTamano(); i++) {
        if (equipos[i] != nullptr) equipos[i]->reactivarJugadores();
    }

    // Continuamos la numeracion despues de los partidos de grupos
    int idPartido = partidos.getTamano() + 1;

    for (int k = 0; k < clasificadosR16.getTamano(); k += 2) {
        Equipo* a = clasificadosR16[k];
        Equipo* b = clasificadosR16[k + 1];
        if (a == nullptr || b == nullptr) continue;

        Partido* p = new Partido(idPartido, R16, a, b);
        idPartido++;

        p->setSede(sedes[rand() % sedes.getTamano()]);
        p->setArbitro(arbitros[rand() % arbitros.getTamano()]);

        fixture.agendarPartidoDesde(p, diaMinimo);
        partidos.agregar(p);
    }

    eliminatoriasGeneradas = true;
    cout << "Partidos de R16 generados (16). Empiezan en dia "
         << diaMinimo << "." << endl;
}

// Simula todos los partidos de la etapa dada (en orden por dia)
// y guarda en 'ganadores' el equipo que gano cada partido.
void Competicion::simularEtapa(Etapa etapa, Lista<Equipo*>& ganadores) {
    ganadores.limpiar();

    // Reactivar jugadores antes de cada ronda
    for (int i = 0; i < equipos.getTamano(); i++) {
        if (equipos[i] != nullptr) equipos[i]->reactivarJugadores();
    }

    // Simular en orden cronologico
    for (int dia = 1; dia <= fixture.getDiaActualMaximo(); dia++) {
        for (int i = 0; i < partidos.getTamano(); i++) {
            Partido* p = partidos[i];
            if (p == nullptr) continue;
            if (p->getEtapa() != etapa) continue;
            if (p->getDia() != dia) continue;
            if (p->yaSimulado) continue;

            p->simular();
            Equipo* gan = p->getEquipoGanador();
            if (gan != nullptr) ganadores.agregar(gan);
        }
    }
}

// Crea partidos para la siguiente etapa emparejando consecutivamente
// la lista de participantes (que viene de los ganadores de la etapa anterior).
void Competicion::crearPartidosSiguienteEtapa(Etapa etapa,
                                              const Lista<Equipo*>& participantes) {
    if (participantes.getTamano() < 2) return;
    if (sedes.getTamano() == 0 || arbitros.getTamano() == 0) return;

    int diaMinimo = fixture.getDiaActualMaximo() + 1;
    int idPartido = partidos.getTamano() + 1;

    for (int k = 0; k < participantes.getTamano(); k += 2) {
        if (k + 1 >= participantes.getTamano()) break;
        Equipo* a = participantes[k];
        Equipo* b = participantes[k + 1];
        if (a == nullptr || b == nullptr) continue;

        Partido* p = new Partido(idPartido, etapa, a, b);
        idPartido++;

        p->setSede(sedes[rand() % sedes.getTamano()]);
        p->setArbitro(arbitros[rand() % arbitros.getTamano()]);

        fixture.agendarPartidoDesde(p, diaMinimo);
        partidos.agregar(p);
    }
}

// Crea el partido por el tercer puesto (perdedores SF1 y SF2)
void Competicion::crearPartidoTercerPuesto(Equipo* perdedorSF1, Equipo* perdedorSF2) {
    if (perdedorSF1 == nullptr || perdedorSF2 == nullptr) return;
    int diaMinimo = fixture.getDiaActualMaximo() + 1;
    int idPartido = partidos.getTamano() + 1;

    Partido* p = new Partido(idPartido, TERCER_PUESTO, perdedorSF1, perdedorSF2);
    p->setSede(sedes[rand() % sedes.getTamano()]);
    p->setArbitro(arbitros[rand() % arbitros.getTamano()]);
    fixture.agendarPartidoDesde(p, diaMinimo);
    partidos.agregar(p);
}

// Orquestador completo de las eliminatorias.
// R16 -> R8 -> QF -> SF -> (3er puesto, FINAL)
void Competicion::simularEliminatorias() {
    if (!gruposSimulados) {
        cout << "Primero hay que simular fase de grupos." << endl;
        return;
    }
    if (eliminatoriasSimuladas) {
        cout << "Las eliminatorias ya se jugaron." << endl;
        return;
    }

    // 1. Seleccionar 32 clasificados (si aun no se hizo)
    if (clasificadosR16.getTamano() == 0) {
        if (!seleccionarClasificadosR16()) return;
    }
    // 2. Generar partidos de R16 (si aun no se hicieron)
    if (!eliminatoriasGeneradas) {
        generarPartidosR16();
    }

    // ===== R16 (32 -> 16) =====
    Lista<Equipo*> ganadoresR16;
    simularEtapa(R16, ganadoresR16);
    cout << "R16 simulado. Ganadores: " << ganadoresR16.getTamano() << endl;

    // ===== R8 = octavos (16 -> 8) =====
    crearPartidosSiguienteEtapa(R8, ganadoresR16);
    Lista<Equipo*> ganadoresR8;
    simularEtapa(R8, ganadoresR8);
    cout << "R8 (octavos) simulado. Ganadores: " << ganadoresR8.getTamano() << endl;

    // ===== QF cuartos (8 -> 4) =====
    crearPartidosSiguienteEtapa(QF, ganadoresR8);
    Lista<Equipo*> ganadoresQF;
    simularEtapa(QF, ganadoresQF);
    cout << "QF simulado. Ganadores: " << ganadoresQF.getTamano() << endl;

    // ===== SF semis (4 -> 2) =====
    crearPartidosSiguienteEtapa(SF, ganadoresQF);
    Lista<Equipo*> ganadoresSF;
    simularEtapa(SF, ganadoresSF);
    cout << "SF simulado. Finalistas: " << ganadoresSF.getTamano() << endl;

    // Para el tercer puesto necesitamos los perdedores de las semis.
    // Buscamos los 2 partidos SF que ya estan jugados.
    Lista<Equipo*> perdedoresSF;
    for (int i = 0; i < partidos.getTamano(); i++) {
        Partido* p = partidos[i];
        if (p == nullptr) continue;
        if (p->getEtapa() != SF) continue;
        Equipo* per = p->getEquipoPerdedor();
        if (per != nullptr) perdedoresSF.agregar(per);
    }

    // ===== Tercer puesto =====
    if (perdedoresSF.getTamano() >= 2) {
        crearPartidoTercerPuesto(perdedoresSF[0], perdedoresSF[1]);
        Lista<Equipo*> ganador3ro;
        simularEtapa(TERCER_PUESTO, ganador3ro);
        if (ganador3ro.getTamano() > 0) {
            tercero = ganador3ro[0];
            // El cuarto es el otro
            cuarto = (perdedoresSF[0] == tercero) ? perdedoresSF[1] : perdedoresSF[0];
        }
    }

    // ===== FINAL =====
    if (ganadoresSF.getTamano() == 2) {
        crearPartidosSiguienteEtapa(FINAL_COPA, ganadoresSF);
        Lista<Equipo*> ganadorFinal;
        simularEtapa(FINAL_COPA, ganadorFinal);
        if (ganadorFinal.getTamano() > 0) {
            campeon = ganadorFinal[0];
            // El sub es el otro finalista
            subcampeon = (ganadoresSF[0] == campeon) ? ganadoresSF[1] : ganadoresSF[0];
        }
    }

    eliminatoriasSimuladas = true;
}

// Imprime el bracket etapa por etapa (todos los partidos eliminatorios)
void Competicion::mostrarBracket() const {
    cout << "===== Bracket Eliminatorias =====" << endl;
    Etapa etapas[] = {R16, R8, QF, SF, TERCER_PUESTO, FINAL_COPA};
    string nombres[] = {"R16 (32->16)", "Octavos (16->8)", "Cuartos (8->4)",
                        "Semifinales", "Tercer Puesto", "FINAL"};
    int cant = 6;

    for (int e = 0; e < cant; e++) {
        cout << "-- " << nombres[e] << " --" << endl;
        for (int i = 0; i < partidos.getTamano(); i++) {
            Partido* p = partidos[i];
            if (p == nullptr) continue;
            if (p->getEtapa() != etapas[e]) continue;
            cout << "   dia " << p->getDia() << " | " << *p << endl;
        }
    }
    cout << "================================" << endl;
}

// Imprime el podio final
void Competicion::mostrarPodio() const {
    cout << "===== PODIO MUNDIAL " << anio << " =====" << endl;
    if (campeon != nullptr) {
        cout << "  CAMPEON:    " << campeon->getCodigoFIFA()
             << " - " << campeon->getNombre() << endl;
    }
    if (subcampeon != nullptr) {
        cout << "  SUB-CAMPEON: " << subcampeon->getCodigoFIFA()
             << " - " << subcampeon->getNombre() << endl;
    }
    if (tercero != nullptr) {
        cout << "  TERCERO:    " << tercero->getCodigoFIFA()
             << " - " << tercero->getNombre() << endl;
    }
    if (cuarto != nullptr) {
        cout << "  CUARTO:     " << cuarto->getCodigoFIFA()
             << " - " << cuarto->getNombre() << endl;
    }
    cout << "===============================" << endl;
}

// Imprime estadisticas finales (top goleador, mas tarjetas, etc.)
void Competicion::mostrarEstadisticasFinales() const {
    if (!eliminatoriasSimuladas) {
        cout << "El torneo aun no termino." << endl;
        return;
    }

    // Buscamos el goleador del torneo
    Jugador* topGol = nullptr;
    int maxGoles = -1;
    int maxAmarillas = -1;
    Jugador* topAmarillas = nullptr;
    int maxRojas = -1;
    Jugador* topRojas = nullptr;

    for (int i = 0; i < equipos.getTamano(); i++) {
        Equipo* e = equipos[i];
        if (e == nullptr) continue;
        for (int j = 0; j < e->getCantidadJugadores(); j++) {
            Jugador* jg = e->getJugador(j);
            if (jg == nullptr || jg->estadisticas == nullptr) continue;
            // El campo 'estadisticas' del Jugador es publico (puntero)
            int gols = jg->estadisticas->getGolesEnMundial();
            int am   = jg->estadisticas->getTarjetasAmarillas();
            int ro   = jg->estadisticas->getTarjetasRojas();
            if (gols > maxGoles)     { maxGoles = gols; topGol = jg; }
            if (am > maxAmarillas)   { maxAmarillas = am; topAmarillas = jg; }
            if (ro > maxRojas)       { maxRojas = ro; topRojas = jg; }
        }
    }

    cout << "===== Estadisticas Finales =====" << endl;
    if (topGol != nullptr && maxGoles > 0) {
        cout << "Maximo goleador: " << *topGol
             << " con " << maxGoles << " goles" << endl;
    } else {
        cout << "Maximo goleador: (no hay goles registrados)" << endl;
    }
    if (topAmarillas != nullptr && maxAmarillas > 0) {
        cout << "Mas tarjetas amarillas: " << *topAmarillas
             << " con " << maxAmarillas << endl;
    }
    if (topRojas != nullptr && maxRojas > 0) {
        cout << "Mas tarjetas rojas: " << *topRojas
             << " con " << maxRojas << endl;
    }
    cout << "Total de partidos jugados: " << partidos.getTamano() << endl;
    cout << "================================" << endl;
}

// main.cpp
// Proyecto: UdeAWorldCup - Desafio II Informatica II 2026-1
// Autor: Anyela Martinez
// Descripcion: Punto de entrada del programa, muestra el menu principal.

#include <iostream>
#include <limits>
#include "tipos.h"
#include "Lista.h"
#include "Jugador.h"
#include "Equipo.h"
#include "Partido.h"
#include "Sede.h"
#include "Arbitro.h"
#include "GestorCSV.h"
#include "MedidorRecursos.h"
#include "Fixture.h"
#include "Competicion.h"
#include <cstdlib>   // srand, rand
#include <ctime>     // time
using namespace std;

// Lista global de equipos del Mundial.
// La declaramos aqui para que las distintas opciones del menu puedan compartirla.
// Mas adelante esto vivira dentro de la clase Competicion.
Lista<Equipo*> g_equipos;

// Competicion global del Mundial.
// La creamos vacia y la vamos llenando con cada opcion del menu.
Competicion g_mundial("FIFA World Cup", 2026);

// Helper para borrar todo lo que tengamos cargado (evita fugas de memoria
// si el usuario carga el CSV varias veces).
// IMPORTANTE: primero limpiamos g_mundial (que tiene partidos que referencian
// a estos equipos como punteros prestados). Asi evitamos que los partidos
// queden con punteros colgantes.
void liberarEquiposGlobales() {
    g_mundial.limpiar();
    for (int i = 0; i < g_equipos.getTamano(); i++) {
        if (g_equipos[i] != nullptr) {
            delete g_equipos[i];
        }
    }
    g_equipos.limpiar();
}

// Funcion que muestra el menu al usuario
// Por ahora solo imprime las opciones, mas adelante iremos llamando a las funcionalidades.
void mostrarMenu() {
    cout << "========================================" << endl;
    cout << "       UdeAWorldCup - Mundial 2026      " << endl;
    cout << "========================================" << endl;
    cout << "1. Cargar/Actualizar datos desde CSV" << endl;
    cout << "2. Conformar grupos del mundial" << endl;
    cout << "3. Simular fase de grupos" << endl;
    cout << "4. Construir tablas y pasar a R16" << endl;
    cout << "5. Simular etapas eliminatorias" << endl;
    cout << "6. Mostrar estadisticas finales" << endl;
    cout << "7. [PRUEBA] Probar clase Equipo" << endl;
    cout << "11. [PRUEBA] Probar clase Partido (Poisson)" << endl;
    cout << "8. [PRUEBA] Probar clase Jugador" << endl;
    cout << "9. [PRUEBA] Probar plantilla Lista<T>" << endl;
    cout << "12. [PRUEBA] Probar clase Fixture (calendario)" << endl;
    cout << "0. Salir" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Opcion: ";
}

// Carga los 48 equipos desde el CSV usando GestorCSV.
// Si ya habia equipos cargados, primero los libera para no duplicar memoria.
void cargarDesdeCSV() {
    liberarEquiposGlobales();

    // RUTA_CSV viene del .pro (DEFINES += RUTA_CSV=...)
    // Por defecto: "../selecciones_clasificadas_mundial.csv"
    string ruta = RUTA_CSV;
    cout << "Cargando equipos desde: " << ruta << endl;

    // Medimos cuanto tarda la carga y cuanta RAM consume
    MedidorRecursos medidor("Carga CSV");
    medidor.iniciar();

    GestorCSV gestor(ruta);
    bool ok = gestor.cargarEquipos(g_equipos);

    medidor.detener();

    if (!ok) {
        cout << "No se pudo abrir el archivo." << endl;
        cout << "Tip: el .pro define RUTA_CSV=\"../selecciones_clasificadas_mundial.csv\"," << endl;
        cout << "asi que el CSV debe estar en la carpeta padre del build." << endl;
        return;
    }

    cout << "Equipos cargados: " << gestor.getCantidadCargada() << endl;
    if (gestor.getLineasIgnoradas() > 0) {
        cout << "Lineas ignoradas (mal formadas): " << gestor.getLineasIgnoradas() << endl;
    }

    // Mostramos los primeros 5 para verificar que se leyo bien
    int aMostrar = (g_equipos.getTamano() < 5) ? g_equipos.getTamano() : 5;
    cout << "Primeros " << aMostrar << " equipos:" << endl;
    for (int i = 0; i < aMostrar; i++) {
        Equipo* e = g_equipos[i];
        if (e != nullptr) {
            cout << "  " << *e
                 << "  GFA=" << e->getPromedioGF()
                 << "  GCB=" << e->getPromedioGC()
                 << endl;
        }
    }

    // Reporte de tiempo y RAM consumidos
    medidor.mostrarReporte();
}

// Prueba rapida de la clase Partido:
// Crea 2 equipos con sus 11 jugadores y simula un partido eliminatoria
// para que se vea la formula de Poisson + posible prorroga + penales.
void probarPartido() {
    cout << "-- Prueba de clase Partido --" << endl;

    // Equipo local
    Equipo* arg = new Equipo("Argentina", "ARG", 1);
    arg->setPromedios(2.1, 0.9);
    for (int i = 1; i <= JUGADORES_CONVOCADOS; i++) {
        arg->agregarJugador(new Jugador(i, "JugARG", "Apellido", i));
    }

    // Equipo visitante
    Equipo* fra = new Equipo("Francia", "FRA", 2);
    fra->setPromedios(1.9, 1.0);
    for (int i = 1; i <= JUGADORES_CONVOCADOS; i++) {
        fra->agregarJugador(new Jugador(i + 100, "JugFRA", "Apellido", i));
    }

    // Sede y arbitro (memoria local del programa, los pasamos por puntero)
    Sede sede("Estadio Azteca", "Mexico DF", "Mexico", 87000);
    Arbitro arbi("Wilton Sampaio", "Brasil");

    // Creamos el partido (ELIMINATORIA: si empata, prorroga + penales)
    Partido p(1, R16, arg, fra);
    p.setSede(&sede);
    p.setArbitro(&arbi);
    p.setDia(1);

    cout << "Antes de simular:" << endl;
    cout << "  " << p << endl;

    p.simular();

    cout << "Despues de simular:" << endl;
    cout << "  " << p << endl;
    cout << "  Sede: " << sede << endl;
    cout << "  Arbitro: " << arbi << endl;

    Equipo* gan = p.getEquipoGanador();
    Equipo* per = p.getEquipoPerdedor();
    if (gan != nullptr) {
        cout << "  Ganador: " << gan->getCodigoFIFA() << endl;
        cout << "  Perdedor: " << per->getCodigoFIFA() << endl;
    } else {
        cout << "  (empate - solo posible en grupos)" << endl;
    }

    // Liberar memoria que creamos aca
    delete arg;
    delete fra;
}

// Prueba rapida de la clase Equipo:
// Crea un equipo, le pone confederacion, federacion, DT y unos jugadores,
// le registra dos resultados y muestra la ficha completa.
void probarEquipo() {
    cout << "-- Prueba de clase Equipo --" << endl;

    // Creamos el equipo
    Equipo arg("Argentina", "ARG", 1);
    arg.setPromedios(2.1, 0.9);
    arg.setTitulosMundiales(3);

    // Confederacion y federacion (las copia por valor)
    ConfederacionContinental conmebol;
    conmebol.nombre = "CONMEBOL";
    conmebol.sigla = "CONMEBOL";
    arg.setConfederacion(conmebol);

    Federacion afa;
    afa.nombre = "AFA";
    arg.setFederacion(afa);

    // DT en memoria dinamica (la clase Equipo se encarga de borrarlo despues)
    DirectorTecnico* dt = new DirectorTecnico();
    dt->nombre = "Lionel Scaloni";
    dt->nacionalidad = "Argentina";
    arg.setDirectorTecnico(dt);

    // Tres jugadores en memoria dinamica
    arg.agregarJugador(new Jugador(1, "Lionel", "Messi", 10));
    arg.agregarJugador(new Jugador(2, "Julian", "Alvarez", 9));
    arg.agregarJugador(new Jugador(3, "Emiliano", "Martinez", 23));

    // Simulamos dos partidos
    arg.registrarResultado(2, 1); // gana
    arg.registrarResultado(0, 0); // empata

    // Mostramos
    cout << arg << endl;
    arg.mostrarPlantilla();
    cout << "Activos antes de roja: " << arg.contarJugadoresActivos() << endl;

    // Le sacamos roja a Messi -> queda inactivo
    arg.getJugador(0)->registrarTarjeta(true);
    cout << "Activos tras roja a Messi: " << arg.contarJugadoresActivos() << endl;

    // Antes del proximo partido reactivamos a todos
    arg.reactivarJugadores();
    cout << "Activos tras reactivar: " << arg.contarJugadoresActivos() << endl;

    // Probamos copia profunda: si modifico la copia, el original NO cambia
    Equipo copia = arg;
    copia.registrarResultado(5, 0); // solo en la copia
    cout << "Original: " << arg << endl;
    cout << "Copia:    " << copia << endl;
    // (al salir de la funcion se llaman los destructores y se libera todo)
}

// Prueba rapida de la clase Jugador:
// - crea un jugador,
// - le registra un gol y una tarjeta roja (queda inactivo),
// - hace una copia profunda y demuestra que son independientes.
void probarJugador() {
    cout << "-- Prueba de clase Jugador --" << endl;
    Jugador j1(1, "Lionel", "Messi", 10);
    cout << "Creado: " << j1 << endl;

    j1.registrarMinutos(90);
    j1.registrarGol(true);   // true = gol en el Mundial
    j1.registrarGol(true);
    j1.registrarFalta();
    cout << "Despues de 2 goles: " << j1 << endl;

    // Probamos copia profunda
    Jugador j2 = j1; // aqui se llama al constructor copia
    cout << "Copia j2: " << j2 << endl;

    // Si cambio j2, j1 no deberia cambiar
    j2.registrarGol(true);
    cout << "Tras sumar gol a j2:" << endl;
    cout << "  j1 = " << j1 << endl;
    cout << "  j2 = " << j2 << endl;

    // Probamos tarjeta roja -> deja al jugador inactivo
    j1.registrarTarjeta(true);
    cout << "Tras roja a j1: " << j1 << endl;
    cout << "Activo? " << (j1.estaActivo() ? "si" : "no") << endl;
}

// Opcion 2 del menu: pasa los equipos a la Competicion, llena bombos,
// carga sedes y arbitros, y hace el sorteo (12 grupos).
void conformarGrupos() {
    if (g_equipos.getTamano() == 0) {
        cout << "Primero carga el CSV (opcion 1)." << endl;
        return;
    }

    MedidorRecursos medidor("Conformar grupos");
    medidor.iniciar();

    // Pasamos los equipos a la Competicion (son prestados, NO se duplican)
    g_mundial.setEquipos(g_equipos);

    // Sedes y arbitros (la primera vez nada mas)
    if (g_mundial.sedes.getTamano() == 0) {
        g_mundial.cargarSedes();
    }
    if (g_mundial.arbitros.getTamano() == 0) {
        g_mundial.cargarArbitros();
    }

    // Bombos por ranking FIFA
    g_mundial.cargarBombos();
    g_mundial.mostrarBombos();

    // Sorteo
    bool ok = g_mundial.realizarSorteo();
    if (!ok) {
        cout << "El sorteo fallo. Revisar las confederaciones." << endl;
        medidor.detener();
        return;
    }

    g_mundial.mostrarGrupos();

    medidor.detener();
    medidor.mostrarReporte();
}

// Opcion 3: genera los 72 partidos de fase de grupos y los simula.
void simularFaseGruposMenu() {
    if (!g_mundial.getSorteoRealizado()) {
        cout << "Primero arma los grupos (opcion 2)." << endl;
        return;
    }
    MedidorRecursos medidor("Fase de Grupos");
    medidor.iniciar();

    g_mundial.generarPartidosFaseGrupos();
    g_mundial.simularFaseGrupos();
    g_mundial.mostrarResultadosFaseGrupos();

    medidor.detener();
    medidor.mostrarReporte();
}

// Opcion 4: arma las tablas, marca clasificados y muestra los 12 grupos.
void construirTablasMenu() {
    MedidorRecursos medidor("Tablas y clasificacion");
    medidor.iniciar();

    g_mundial.construirTablasYClasificar();
    g_mundial.mostrarTablasGrupos();

    medidor.detener();
    medidor.mostrarReporte();
}

// Opcion 5: simula todas las eliminatorias (R16 -> R8 -> QF -> SF -> 3o + Final)
void simularEliminatoriasMenu() {
    MedidorRecursos medidor("Eliminatorias");
    medidor.iniciar();

    g_mundial.simularEliminatorias();
    g_mundial.mostrarBracket();
    g_mundial.mostrarPodio();

    medidor.detener();
    medidor.mostrarReporte();
}

// Opcion 6: muestra las estadisticas finales del torneo.
void mostrarEstadisticasFinalesMenu() {
    g_mundial.mostrarPodio();
    g_mundial.mostrarEstadisticasFinales();
}

// Prueba rapida de Fixture: arma 6 partidos entre 4 equipos y verifica
// que se respeten las reglas (max 4 partidos/dia y 3 dias de descanso).
void probarFixture() {
    cout << "-- Prueba de clase Fixture --" << endl;

    // 4 equipos de juguete
    Equipo* a = new Equipo("Argentina", "ARG", 1);
    Equipo* b = new Equipo("Brasil", "BRA", 2);
    Equipo* c = new Equipo("Colombia", "COL", 3);
    Equipo* d = new Equipo("Uruguay", "URU", 4);

    // 6 partidos para que se vean varios dias
    Partido* p1 = new Partido(1, FASE_GRUPOS, a, b);
    Partido* p2 = new Partido(2, FASE_GRUPOS, c, d);
    Partido* p3 = new Partido(3, FASE_GRUPOS, a, c);
    Partido* p4 = new Partido(4, FASE_GRUPOS, b, d);
    Partido* p5 = new Partido(5, FASE_GRUPOS, a, d);
    Partido* p6 = new Partido(6, FASE_GRUPOS, b, c);

    Fixture fix("Prueba grupo de 4");
    fix.agendarPartido(p1);
    fix.agendarPartido(p2);
    fix.agendarPartido(p3);
    fix.agendarPartido(p4);
    fix.agendarPartido(p5);
    fix.agendarPartido(p6);

    fix.mostrarCalendario();

    cout << "Ultimo dia ARG: " << fix.ultimoDiaDeEquipo(a) << endl;
    cout << "Ultimo dia BRA: " << fix.ultimoDiaDeEquipo(b) << endl;

    // Liberamos lo que creamos aca
    delete p1; delete p2; delete p3; delete p4; delete p5; delete p6;
    delete a; delete b; delete c; delete d;
}

// Prueba rapida de la plantilla Lista<T> con enteros.
// Solo para verificar que agregar, operator[] y eliminar funcionan bien.
void probarLista() {
    cout << "-- Prueba de Lista<int> --" << endl;
    Lista<int> numeros;
    numeros.agregar(10);
    numeros.agregar(20);
    numeros.agregar(30);
    numeros.agregar(40);

    cout << "Tamano = " << numeros.getTamano() << endl;
    for (int i = 0; i < numeros.getTamano(); i++) {
        cout << "  numeros[" << i << "] = " << numeros[i] << endl;
    }

    cout << "Eliminando el elemento en indice 1..." << endl;
    numeros.eliminar(1);
    for (int i = 0; i < numeros.getTamano(); i++) {
        cout << "  numeros[" << i << "] = " << numeros[i] << endl;
    }
    cout << "Lista vacia? " << (numeros.estaVacia() ? "si" : "no") << endl;
}

int main() {
    // Inicializamos la semilla aleatoria una sola vez al arrancar el programa
    // (necesaria para rand() en la simulacion de Poisson y penales).
    srand((unsigned int)time(nullptr));

    int opcion = -1;

    // Bucle principal del menu
    // Lo dejamos lo mas simple posible, mas adelante conectaremos cada opcion.
    while (opcion != 0) {
        mostrarMenu();
        cin >> opcion;

        // Si el usuario escribe algo que no es numero, cin entra en estado de error
        // y empieza a devolver el mismo valor sin esperar input -> ciclo infinito.
        // Limpiamos el estado de error y descartamos lo que haya en el buffer.
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada invalida. Por favor escribe un numero." << endl;
            continue;
        }

        if (opcion == 0) {
            cout << "Saliendo del sistema..." << endl;
            // Liberamos la memoria de los equipos antes de cerrar
            liberarEquiposGlobales();
        } else if (opcion == 1) {
            cargarDesdeCSV();
        } else if (opcion == 2) {
            conformarGrupos();
        } else if (opcion == 3) {
            simularFaseGruposMenu();
        } else if (opcion == 4) {
            construirTablasMenu();
        } else if (opcion == 5) {
            simularEliminatoriasMenu();
        } else if (opcion == 6) {
            mostrarEstadisticasFinalesMenu();
        } else if (opcion == 7) {
            probarEquipo();
        } else if (opcion == 11) {
            probarPartido();
        } else if (opcion == 8) {
            probarJugador();
        } else if (opcion == 9) {
            probarLista();
        } else if (opcion == 12) {
            probarFixture();
        } else {
            // Por ahora solo avisamos que la opcion no esta implementada.
            cout << "[Opcion " << opcion << "] pendiente de implementar." << endl;
        }
        cout << endl;
    }

    return 0;
}

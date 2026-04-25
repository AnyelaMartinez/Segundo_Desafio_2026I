// main.cpp
// Proyecto: UdeAWorldCup - Desafio II Informatica II 2026-1
// Autor: Anyela Martinez
// Descripcion: Punto de entrada del programa, muestra el menu principal.

#include <iostream>
#include <limits>
#include "tipos.h"
#include "Lista.h"
#include "Jugador.h"
using namespace std;

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
    cout << "8. [PRUEBA] Probar clase Jugador" << endl;
    cout << "9. [PRUEBA] Probar plantilla Lista<T>" << endl;
    cout << "0. Salir" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Opcion: ";
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
        } else if (opcion == 8) {
            probarJugador();
        } else if (opcion == 9) {
            probarLista();
        } else {
            // Por ahora solo avisamos que la opcion no esta implementada.
            cout << "[Opcion " << opcion << "] pendiente de implementar." << endl;
        }
        cout << endl;
    }

    return 0;
}

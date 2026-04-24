// main.cpp
// Proyecto: UdeAWorldCup - Desafio II Informatica II 2026-1
// Autor: Anyela Martinez
// Descripcion: Punto de entrada del programa, muestra el menu principal.

#include <iostream>
#include "tipos.h"
#include "Lista.h"
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
    cout << "9. [PRUEBA] Probar plantilla Lista<T>" << endl;
    cout << "0. Salir" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Opcion: ";
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

        if (opcion == 0) {
            cout << "Saliendo del sistema..." << endl;
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

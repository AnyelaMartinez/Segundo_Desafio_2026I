// Lista.h
// Plantilla (template) Lista<T>: reemplaza a std::vector, que esta prohibido
// por el desafio. Usa memoria dinamica propia (arreglo T* datos).
//
// Como es una plantilla, TODO va en el header (no hay Lista.cpp).
// Asi el compilador puede generar la version de Lista<int>, Lista<Equipo*>, etc.

#ifndef LISTA_H
#define LISTA_H

#include <iostream>
using namespace std;

template <class T>
class Lista {
private:
    T* datos;        // arreglo dinamico que guarda los elementos
    int tamano;      // cuantos elementos tiene la lista ahora
    int capacidad;   // cuanto espacio tiene reservado (puede ser > tamano)

    // Metodo privado auxiliar: duplica la capacidad cuando la lista se llena.
    // Copia los datos viejos al arreglo nuevo y libera el anterior.
    void redimensionar() {
        int nuevaCap = capacidad * 2;
        if (nuevaCap == 0) nuevaCap = 4; // si estaba vacia, empieza con 4
        T* nuevoArreglo = new T[nuevaCap];
        for (int i = 0; i < tamano; i++) {
            nuevoArreglo[i] = datos[i];
        }
        delete[] datos;
        datos = nuevoArreglo;
        capacidad = nuevaCap;
    }

public:
    // Constructor por defecto: lista vacia, sin reservar memoria todavia.
    Lista() {
        datos = nullptr;
        tamano = 0;
        capacidad = 0;
    }

    // Constructor de copia: hace una copia independiente de otra lista.
    // IMPORTANTE para cuando se pasa una lista por valor (aunque preferimos
    // pasarla por referencia para eficiencia).
    Lista(const Lista<T>& otra) {
        tamano = otra.tamano;
        capacidad = otra.capacidad;
        if (capacidad > 0) {
            datos = new T[capacidad];
            for (int i = 0; i < tamano; i++) {
                datos[i] = otra.datos[i];
            }
        } else {
            datos = nullptr;
        }
    }

    // Destructor: libera la memoria reservada.
    ~Lista() {
        delete[] datos;
    }

    // Agrega un elemento al final de la lista. Si no hay espacio, redimensiona.
    void agregar(T dato) {
        if (tamano == capacidad) {
            redimensionar();
        }
        datos[tamano] = dato;
        tamano++;
    }

    // Devuelve una referencia al elemento en la posicion indicada.
    // Usamos referencia para no hacer copias innecesarias (criterio de eficiencia).
    T& obtener(int indice) {
        // Si el indice esta fuera de rango, devolvemos el primero para no crashear.
        // En un programa de estudiante esto suele ser suficiente.
        if (indice < 0 || indice >= tamano) {
            cout << "Advertencia: indice " << indice << " fuera de rango." << endl;
            return datos[0];
        }
        return datos[indice];
    }

    // Version CONST de obtener: cuando la lista es const (por ejemplo
    // dentro de un metodo const o de un constructor copia que recibe
    // 'const Lista&'), C++ elige esta version.
    const T& obtener(int indice) const {
        if (indice < 0 || indice >= tamano) {
            cout << "Advertencia: indice " << indice << " fuera de rango." << endl;
            return datos[0];
        }
        return datos[indice];
    }

    // Elimina el elemento en la posicion indicada, corriendo los siguientes.
    void eliminar(int indice) {
        if (indice < 0 || indice >= tamano) return;
        for (int i = indice; i < tamano - 1; i++) {
            datos[i] = datos[i + 1];
        }
        tamano--;
    }

    // Consulta cuantos elementos hay.
    int getTamano() const {
        return tamano;
    }

    // Dice si la lista esta vacia.
    bool estaVacia() const {
        return tamano == 0;
    }

    // Deja la lista vacia (no libera la memoria, solo reinicia el contador).
    void limpiar() {
        tamano = 0;
    }

    // Sobrecarga del operador []: permite usar lista[i] como si fuera un arreglo.
    T& operator[](int i) {
        return obtener(i);
    }

    // Version CONST del operador []: para usar lista[i] desde contextos const.
    const T& operator[](int i) const {
        return obtener(i);
    }

    // Sobrecarga del operador =: asignar una lista a otra.
    // Primero libera lo viejo y luego copia lo nuevo.
    Lista<T>& operator=(const Lista<T>& otra) {
        if (this == &otra) return *this; // proteccion contra auto-asignacion
        delete[] datos;
        tamano = otra.tamano;
        capacidad = otra.capacidad;
        if (capacidad > 0) {
            datos = new T[capacidad];
            for (int i = 0; i < tamano; i++) {
                datos[i] = otra.datos[i];
            }
        } else {
            datos = nullptr;
        }
        return *this;
    }
};

#endif // LISTA_H

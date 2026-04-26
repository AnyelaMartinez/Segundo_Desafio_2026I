// GestorCSV.h
// Clase encargada de leer el archivo CSV con las 48 selecciones
// clasificadas al Mundial 2026 y crear los Equipo correspondientes.
//
// Formato esperado (separador ';'):
//   Linea 1: titulo libre (la ignoramos)
//   Linea 2: cabecera de columnas (la ignoramos)
//   Linea 3 en adelante:
//     ranking;pais;DT;federacion;confederacion;GF;GC;PG;PE;PP
//
// El gestor SOLO crea los Equipo (con new) y los agrega a la lista que
// le pasen. La responsabilidad de borrarlos es de quien los use despues
// (la clase Competicion).

#ifndef GESTORCSV_H
#define GESTORCSV_H

#include <iostream>
#include <string>
#include "Lista.h"
#include "Equipo.h"

using namespace std;

class GestorCSV {
public:
    string rutaArchivo;
    int ultimaCantidadCargada;     // cuantos equipos cargo en el ultimo intento
    int ultimasLineasIgnoradas;    // cuantas lineas se saltaron por error

    GestorCSV();
    GestorCSV(string _ruta);
    GestorCSV(const GestorCSV& otro);

    void setRuta(string r);
    string getRuta() const;
    int getCantidadCargada() const;
    int getLineasIgnoradas() const;

    // Lee el archivo y agrega los equipos a 'destino'. Devuelve true si pudo
    // abrir el archivo, false si no.
    // OJO: el llamador debe luego asociar los DT con jugadores ficticios
    // (porque el CSV no trae plantilla). Eso lo hace Competicion.
    bool cargarEquipos(Lista<Equipo*>& destino);

private:
    // Parte una linea por el separador ';' y guarda los campos en 'salida'.
    // Devuelve la cantidad de campos encontrados.
    int dividirLinea(const string& linea, char separador,
                     string salida[], int maxCampos) const;

    // Convierte un string a entero. Si esta mal, devuelve 0.
    int aEntero(const string& s) const;

    // Quita espacios al inicio y al final
    string recortar(const string& s) const;

    // Genera un codigo FIFA aproximado a partir del nombre del pais.
    // Toma las 3 primeras letras y las pasa a mayuscula. No es 100% el
    // codigo oficial pero sirve para identificar el equipo.
    string generarCodigoFIFA(const string& pais) const;
};

#endif // GESTORCSV_H

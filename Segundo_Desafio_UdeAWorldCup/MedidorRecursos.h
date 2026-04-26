// MedidorRecursos.h
// Clase utilitaria para medir tiempo de ejecucion y RAM usada por el programa.
// Es uno de los requisitos del desafio: que el sistema reporte cuanto tarda
// y cuanta memoria consume.
//
// Tiempo: usamos <chrono> de la libreria estandar.
// RAM: usamos la API de Windows (psapi) - el proyecto agrega -lpsapi en el .pro.
//      En otros sistemas devuelve 0 (no nos preocupamos por linux/mac aca).

#ifndef MEDIDORRECURSOS_H
#define MEDIDORRECURSOS_H

#include <iostream>
#include <string>
using namespace std;

class MedidorRecursos {
public:
    string nombreSeccion;          // que estamos midiendo (ej: "fase de grupos")

    long long tiempoInicioMs;      // marca de tiempo cuando se llamo iniciar()
    long long tiempoFinMs;         // marca de tiempo cuando se llamo detener()
    bool corriendo;                // true entre iniciar() y detener()

    long long ramAlIniciarBytes;   // RAM en el momento de iniciar
    long long ramAlDetenerBytes;   // RAM en el momento de detener

    MedidorRecursos();
    MedidorRecursos(string _nombre);
    MedidorRecursos(const MedidorRecursos& otro);

    // Marca el inicio: guarda tiempo y RAM actuales.
    void iniciar();

    // Marca el fin: guarda tiempo y RAM actuales.
    void detener();

    // Resetea para volver a empezar
    void reiniciar();

    // Tiempo transcurrido entre iniciar() y detener() en milisegundos.
    // Si todavia esta corriendo, calcula desde 'ahora'.
    long long getTiempoMs() const;
    double getTiempoSegundos() const;

    // Cuanta RAM consumio aproximadamente esta seccion (delta).
    // Puede ser negativo si se libero memoria, lo dejamos pasar.
    long long getDeltaRAMBytes() const;
    double getDeltaRAMKB() const;
    double getDeltaRAMMB() const;

    // Imprime el reporte de tiempo + RAM.
    void mostrarReporte() const;

    // ========= Metodos estaticos (utilidades de bajo nivel) =========
    // Estos no necesitan instancia: se pueden llamar como
    //   MedidorRecursos::getRAMActualBytes();

    // RAM ocupada por el proceso actual, en bytes.
    // En Windows usa GetProcessMemoryInfo. En otros SOs devuelve 0.
    static long long getRAMActualBytes();

    // Marca de tiempo actual en milisegundos (con base en steady_clock).
    static long long getTiempoActualMs();

    // Operador == : comparamos por nombre de seccion
    bool operator==(const MedidorRecursos& otro) const;

    friend ostream& operator<<(ostream& os, const MedidorRecursos& m);
};

#endif // MEDIDORRECURSOS_H

// EstadisticasPartido.h
// Estadisticas resumen de UN partido (no acumulado de torneo).
// Es lo que veriamos al final del partido en una "ficha del partido".
//
// OJO: aqui guardamos los totales del partido (ambos equipos juntos
// para tarjetas, faltas, espectadores...), pero los goles si los
// separamos por local/visitante porque son la informacion clave.
//
// Esta clase NO tiene memoria dinamica, asi que la regla de los 3
// no aplica realmente, pero igual escribimos el constructor copia
// para cumplir con lo que pide el desafio.

#ifndef ESTADISTICASPARTIDO_H
#define ESTADISTICASPARTIDO_H

#include <iostream>
#include <string>
using namespace std;

class EstadisticasPartido {
public:
    int golesLocal;
    int golesVisitante;
    int totalTarjetasAmarillas;
    int totalTarjetasRojas;
    int totalFaltas;

    int duracionMinutos;     // 90 normal, 120 si hubo prorroga
    bool huboProrroga;
    bool huboPenales;        // solo en eliminatorias

    // Resultado de los penales (si aplica). -1 si no hubo.
    int penalesLocal;
    int penalesVisitante;

    int espectadores;        // simulamos un valor segun el aforo

    EstadisticasPartido();
    EstadisticasPartido(const EstadisticasPartido& otro);
    EstadisticasPartido& operator=(const EstadisticasPartido& otro);

    // Setters basicos
    void setGoles(int gl, int gv);
    void setProrroga(bool b);
    void setPenales(int pl, int pv);
    void setDuracion(int min);
    void setEspectadores(int n);

    // Para ir sumando eventos durante la simulacion del partido
    void sumarTarjetaAmarilla();
    void sumarTarjetaRoja();
    void sumarFalta();

    // Getters
    int getGolesLocal() const;
    int getGolesVisitante() const;
    int getTotalTarjetas() const;   // suma amarillas + rojas
    int getTotalFaltas() const;
    int getDuracion() const;
    bool getHuboProrroga() const;
    bool getHuboPenales() const;

    // Devuelve "L" si gano el local, "V" si gano el visitante, "E" si empate.
    // En caso de penales, considera al ganador segun penales.
    string getGanadorSimbolo() const;

    // Resetea todos los contadores
    void reiniciar();

    friend ostream& operator<<(ostream& os, const EstadisticasPartido& e);
};

#endif // ESTADISTICASPARTIDO_H

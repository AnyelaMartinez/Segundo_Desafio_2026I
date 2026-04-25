// EstadisticasJugadorPartido.h
// Snapshot de UN jugador en UN partido especifico.
// Se crea uno nuevo para cada jugador convocado en cada partido.
// Al terminar el partido estos valores se vuelcan sobre EstadisticasJugador.

#ifndef ESTADISTICASJUGADORPARTIDO_H
#define ESTADISTICASJUGADORPARTIDO_H

class EstadisticasJugadorPartido {
public:
    int goles;
    int tarjetasRojas;
    int tarjetasAmarillas;
    int faltas;
    int minutosJugados;   // 90 normal, 120 si hubo prorroga

    EstadisticasJugadorPartido();
    EstadisticasJugadorPartido(const EstadisticasJugadorPartido& otro);

    // Metodos para registrar eventos uno a uno durante la simulacion.
    void registrarGol();
    void registrarAmarilla();
    void registrarRoja();
    void registrarFalta();
    void setMinutos(int m);

    int getGoles() const;
    int getAmarillas() const;
    int getRojas() const;
    int getFaltas() const;
    int getMinutos() const;

    void reiniciar();
};

#endif // ESTADISTICASJUGADORPARTIDO_H

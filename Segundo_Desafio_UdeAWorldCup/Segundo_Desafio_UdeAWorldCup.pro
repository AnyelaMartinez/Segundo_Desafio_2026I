TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

# Proyecto UdeAWorldCup - Desafio II Informatica II
# Simulacion del Mundial de Futbol FIFA 2026

SOURCES += \
        main.cpp \
        ConfederacionContinental.cpp \
        Federacion.cpp \
        DirectorTecnico.cpp \
        EstadisticasJugador.cpp \
        EstadisticasJugadorPartido.cpp \
        Jugador.cpp \
        EstadisticasEquipo.cpp \
        Equipo.cpp \
        Sede.cpp \
        Arbitro.cpp \
        EstadisticasPartido.cpp \
        ParticipacionEquipo.cpp \
        Partido.cpp \
        PosicionGrupo.cpp \
        Bombo.cpp \
        Grupo.cpp \
        GestorCSV.cpp \
        MedidorRecursos.cpp \
        Fixture.cpp \
        Competicion.cpp

HEADERS += \
        tipos.h \
        Lista.h \
        ConfederacionContinental.h \
        Federacion.h \
        DirectorTecnico.h \
        EstadisticasJugador.h \
        EstadisticasJugadorPartido.h \
        Jugador.h \
        EstadisticasEquipo.h \
        Equipo.h \
        Sede.h \
        Arbitro.h \
        EstadisticasPartido.h \
        ParticipacionEquipo.h \
        Partido.h \
        PosicionGrupo.h \
        Bombo.h \
        Grupo.h \
        GestorCSV.h \
        MedidorRecursos.h \
        Fixture.h \
        Competicion.h

# En Windows necesitamos psapi para medir RAM (GetProcessMemoryInfo).
# Si compilas en Linux/Mac esta linea no afecta porque MedidorRecursos
# usa #ifdef _WIN32 y no llama a la API.
win32: LIBS += -lpsapi

# Para que Qt Creator encuentre el CSV sin cambiar de carpeta
DEFINES += RUTA_CSV=\\\"../selecciones_clasificadas_mundial.csv\\\"

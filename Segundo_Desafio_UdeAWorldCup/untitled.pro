TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

# Proyecto UdeAWorldCup - Desafio II Informatica II
# Simulacion del Mundial de Futbol FIFA 2026

SOURCES += \
        main.cpp

HEADERS += \
        tipos.h \
        Lista.h

# Para que Qt Creator encuentre el CSV sin cambiar de carpeta
DEFINES += RUTA_CSV=\\\"../selecciones_clasificadas_mundial.csv\\\"

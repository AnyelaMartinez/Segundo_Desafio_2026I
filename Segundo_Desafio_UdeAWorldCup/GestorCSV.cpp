// GestorCSV.cpp
// Implementacion de la clase GestorCSV.

#include "GestorCSV.h"
#include "Federacion.h"
#include "ConfederacionContinental.h"
#include "DirectorTecnico.h"

#include <fstream>
#include <cctype>     // toupper

GestorCSV::GestorCSV() {
    rutaArchivo = "";
    ultimaCantidadCargada = 0;
    ultimasLineasIgnoradas = 0;
}

GestorCSV::GestorCSV(string _ruta) {
    rutaArchivo = _ruta;
    ultimaCantidadCargada = 0;
    ultimasLineasIgnoradas = 0;
}

GestorCSV::GestorCSV(const GestorCSV& otro) {
    rutaArchivo = otro.rutaArchivo;
    ultimaCantidadCargada = otro.ultimaCantidadCargada;
    ultimasLineasIgnoradas = otro.ultimasLineasIgnoradas;
}

void GestorCSV::setRuta(string r)        { rutaArchivo = r; }
string GestorCSV::getRuta() const        { return rutaArchivo; }
int GestorCSV::getCantidadCargada() const { return ultimaCantidadCargada; }
int GestorCSV::getLineasIgnoradas() const { return ultimasLineasIgnoradas; }

// Quita espacios al inicio y al final.
string GestorCSV::recortar(const string& s) const {
    int ini = 0;
    int fin = static_cast<int>(s.size()) - 1;
    while (ini <= fin && (s[ini] == ' ' || s[ini] == '\t' || s[ini] == '\r')) ini++;
    while (fin >= ini && (s[fin] == ' ' || s[fin] == '\t' || s[fin] == '\r')) fin--;
    if (ini > fin) return "";
    return s.substr(ini, fin - ini + 1);
}

// Convierte string a entero. Si esta mal, devuelve 0.
int GestorCSV::aEntero(const string& s) const {
    string t = recortar(s);
    if (t.empty()) return 0;
    int valor = 0;
    int i = 0;
    int signo = 1;
    if (t[0] == '-') { signo = -1; i = 1; }
    for (; i < static_cast<int>(t.size()); i++) {
        if (t[i] < '0' || t[i] > '9') break;
        valor = valor * 10 + (t[i] - '0');
    }
    return valor * signo;
}

// Divide una linea por el separador. Devuelve cuantos campos halló.
int GestorCSV::dividirLinea(const string& linea, char separador,
                            string salida[], int maxCampos) const {
    int cnt = 0;
    string actual = "";
    for (int i = 0; i < static_cast<int>(linea.size()); i++) {
        char c = linea[i];
        if (c == separador) {
            if (cnt < maxCampos) {
                salida[cnt] = recortar(actual);
                cnt++;
            }
            actual = "";
        } else {
            actual += c;
        }
    }
    // Ultimo campo (despues del ultimo ';')
    if (cnt < maxCampos) {
        salida[cnt] = recortar(actual);
        cnt++;
    }
    return cnt;
}

// Genera un codigo de 3 letras en mayuscula a partir del nombre del pais.
// Ej: "France" -> "FRA", "Argentina" -> "ARG".
string GestorCSV::generarCodigoFIFA(const string& pais) const {
    string r = "";
    int n = static_cast<int>(pais.size());
    int i = 0;
    while (i < n && r.size() < 3) {
        char c = pais[i];
        if (c != ' ') {
            r += static_cast<char>(toupper(static_cast<unsigned char>(c)));
        }
        i++;
    }
    while (r.size() < 3) {
        r += '?';
    }
    return r;
}

// Lee el archivo, parsea cada linea y crea los Equipo correspondientes.
// El llamador es responsable de hacer 'delete' de los Equipo despues.
bool GestorCSV::cargarEquipos(Lista<Equipo*>& destino) {
    ultimaCantidadCargada = 0;
    ultimasLineasIgnoradas = 0;

    ifstream archivo(rutaArchivo.c_str());
    if (!archivo.is_open()) {
        cout << "ERROR: no se pudo abrir el archivo: " << rutaArchivo << endl;
        return false;
    }

    string linea;
    int numeroLinea = 0;
    const int MAX_CAMPOS = 12;
    string campos[MAX_CAMPOS];

    while (getline(archivo, linea)) {
        numeroLinea++;
        // Saltamos las dos primeras lineas (titulo + cabecera)
        if (numeroLinea <= 2) continue;
        // Saltamos lineas vacias
        if (recortar(linea).empty()) continue;

        int n = dividirLinea(linea, ';', campos, MAX_CAMPOS);
        // Esperamos al menos 10 campos
        if (n < 10) {
            ultimasLineasIgnoradas++;
            continue;
        }

        // Mapeo de columnas:
        //   0: ranking
        //   1: pais
        //   2: DT
        //   3: federacion
        //   4: confederacion
        //   5: GF
        //   6: GC
        //   7: PG
        //   8: PE
        //   9: PP
        int ranking         = aEntero(campos[0]);
        string pais         = campos[1];
        string nombreDT     = campos[2];
        string nombreFed    = campos[3];
        string siglaConf    = campos[4];
        int golesFavor      = aEntero(campos[5]);
        int golesContra     = aEntero(campos[6]);
        int pg              = aEntero(campos[7]);
        int pe              = aEntero(campos[8]);
        int pp              = aEntero(campos[9]);

        // Si el pais quedo vacio, no podemos hacer nada con esta linea
        if (pais.empty()) {
            ultimasLineasIgnoradas++;
            continue;
        }

        // Calculamos los promedios para la formula de Poisson.
        // Si por algun motivo no hubo partidos, dejamos 0 y la formula
        // luego usa el valor por defecto (mu).
        int totalPartidos = pg + pe + pp;
        double gfaProm = 0.0;
        double gcbProm = 0.0;
        if (totalPartidos > 0) {
            gfaProm = static_cast<double>(golesFavor) / static_cast<double>(totalPartidos);
            gcbProm = static_cast<double>(golesContra) / static_cast<double>(totalPartidos);
        }

        // Creamos el equipo
        string codigo = generarCodigoFIFA(pais);
        Equipo* eq = new Equipo(pais, codigo, ranking);
        eq->setPromedios(gfaProm, gcbProm);

        // Confederacion (sigla = lo que viene del CSV, nombre tambien)
        ConfederacionContinental conf;
        conf.nombre = siglaConf;
        conf.sigla = siglaConf;
        eq->setConfederacion(conf);

        // Federacion (solo nombre)
        Federacion fed;
        fed.nombre = nombreFed;
        eq->setFederacion(fed);

        // Director Tecnico (memoria dinamica, el Equipo lo libera despues)
        DirectorTecnico* dt = new DirectorTecnico();
        dt->nombre = nombreDT;
        dt->nacionalidad = pais; // asumimos que el DT es del mismo pais
        eq->setDirectorTecnico(dt);

        destino.agregar(eq);
        ultimaCantidadCargada++;
    }

    archivo.close();
    return true;
}

// MedidorRecursos.cpp
// Implementacion de MedidorRecursos.

#include "MedidorRecursos.h"
#include <chrono>

// En Windows usamos psapi.h para consultar la RAM del proceso.
// En Qt para que enlace bien necesitamos en el .pro:  LIBS += -lpsapi
//
// IMPORTANTE (workaround conocido MinGW + C++17):
// Los headers de Windows (rpcndr.h, objidl.h, wtypes.h) definen un tipo
// 'byte' en el espacio de nombres global que CHOCA con 'std::byte' de C++17.
// Renombramos 'byte' a un nombre temporal antes de incluir los headers de
// Windows para evitar el error "reference to 'byte' is ambiguous".
// Esto NO afecta nuestro codigo porque nosotros nunca usamos 'byte'.
#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #define byte win_byte_workaround
    #include <windows.h>
    #include <psapi.h>
    #undef byte
#endif

using namespace std;

MedidorRecursos::MedidorRecursos() {
    nombreSeccion = "";
    tiempoInicioMs = 0;
    tiempoFinMs = 0;
    corriendo = false;
    ramAlIniciarBytes = 0;
    ramAlDetenerBytes = 0;
}

MedidorRecursos::MedidorRecursos(string _nombre) {
    nombreSeccion = _nombre;
    tiempoInicioMs = 0;
    tiempoFinMs = 0;
    corriendo = false;
    ramAlIniciarBytes = 0;
    ramAlDetenerBytes = 0;
}

MedidorRecursos::MedidorRecursos(const MedidorRecursos& otro) {
    nombreSeccion = otro.nombreSeccion;
    tiempoInicioMs = otro.tiempoInicioMs;
    tiempoFinMs = otro.tiempoFinMs;
    corriendo = otro.corriendo;
    ramAlIniciarBytes = otro.ramAlIniciarBytes;
    ramAlDetenerBytes = otro.ramAlDetenerBytes;
}

void MedidorRecursos::iniciar() {
    tiempoInicioMs = getTiempoActualMs();
    ramAlIniciarBytes = getRAMActualBytes();
    corriendo = true;
}

void MedidorRecursos::detener() {
    tiempoFinMs = getTiempoActualMs();
    ramAlDetenerBytes = getRAMActualBytes();
    corriendo = false;
}

void MedidorRecursos::reiniciar() {
    tiempoInicioMs = 0;
    tiempoFinMs = 0;
    corriendo = false;
    ramAlIniciarBytes = 0;
    ramAlDetenerBytes = 0;
}

long long MedidorRecursos::getTiempoMs() const {
    if (corriendo) {
        return getTiempoActualMs() - tiempoInicioMs;
    }
    return tiempoFinMs - tiempoInicioMs;
}

double MedidorRecursos::getTiempoSegundos() const {
    return static_cast<double>(getTiempoMs()) / 1000.0;
}

long long MedidorRecursos::getDeltaRAMBytes() const {
    return ramAlDetenerBytes - ramAlIniciarBytes;
}

double MedidorRecursos::getDeltaRAMKB() const {
    return static_cast<double>(getDeltaRAMBytes()) / 1024.0;
}

double MedidorRecursos::getDeltaRAMMB() const {
    return static_cast<double>(getDeltaRAMBytes()) / (1024.0 * 1024.0);
}

void MedidorRecursos::mostrarReporte() const {
    cout << "------ Reporte de recursos ------" << endl;
    if (!nombreSeccion.empty()) {
        cout << "Seccion: " << nombreSeccion << endl;
    }
    cout << "Tiempo: " << getTiempoMs() << " ms ("
         << getTiempoSegundos() << " s)" << endl;
    cout << "RAM al iniciar: "
         << (static_cast<double>(ramAlIniciarBytes) / (1024.0 * 1024.0)) << " MB" << endl;
    cout << "RAM al detener: "
         << (static_cast<double>(ramAlDetenerBytes) / (1024.0 * 1024.0)) << " MB" << endl;
    cout << "Delta RAM:      " << getDeltaRAMKB() << " KB ("
         << getDeltaRAMMB() << " MB)" << endl;
    cout << "---------------------------------" << endl;
}

// ===== Metodos estaticos =====

long long MedidorRecursos::getTiempoActualMs() {
    using namespace std::chrono;
    auto ahora = steady_clock::now();
    auto dur = duration_cast<milliseconds>(ahora.time_since_epoch());
    return static_cast<long long>(dur.count());
}

long long MedidorRecursos::getRAMActualBytes() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc;
    HANDLE proceso = GetCurrentProcess();
    if (GetProcessMemoryInfo(proceso, &pmc, sizeof(pmc))) {
        // WorkingSetSize = memoria fisica que el proceso tiene en RAM
        return static_cast<long long>(pmc.WorkingSetSize);
    }
    return 0;
#else
    // En linux/mac no implementamos, devolvemos 0.
    return 0;
#endif
}

bool MedidorRecursos::operator==(const MedidorRecursos& otro) const {
    return nombreSeccion == otro.nombreSeccion;
}

ostream& operator<<(ostream& os, const MedidorRecursos& m) {
    os << "[Medidor " << m.nombreSeccion << "] "
       << "tiempo=" << m.getTiempoMs() << "ms"
       << " deltaRAM=" << m.getDeltaRAMKB() << "KB";
    return os;
}

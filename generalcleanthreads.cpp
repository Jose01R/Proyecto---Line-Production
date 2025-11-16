#include "generalcleanthreads.h"
#include <QDebug>
#include <QRandomGenerator>

/**
 * @brief Ciclo principal del hilo GeneralCleanThreads.
 * Simula un proceso de limpieza que se ejecuta cada cierto tiempo.
 */
void GeneralCleanThreads::run() {
    // Indica el inicio del funcionamiento
    emitMessage("INICIO", "Hilo de limpieza activado y funcionando.");

    // El ciclo se repite mientras isRunning sea true
    while (true) {
        {
            QMutexLocker locker(&mutex);
            if (!isRunning) break; // Salir si se ha solicitado la detención
        }

        // --- SIMULACIÓN DE ACCIÓN DE MANTENIMIENTO ---
        // 1. Espera de 15 a 30 segundos (simulando un intervalo periódico)
        int delay = QRandomGenerator::global()->bounded(15000, 30001); // 15s a 30s
        msleep(delay);

        {
            QMutexLocker locker(&mutex);
            if (!isRunning) break;
        }

        // 2. Ejecutar la acción de limpieza
        QString action = "Realizando reseteo y reactivación de recursos (Simulado).";
        emitMessage("ADVERTENCIA", action);

        // 3. Emitir la señal para que la clase principal realice el trabajo real
        emit requestSystemReset();

        // 4. Pausa para simular la finalización de la limpieza
        msleep(2000); // Pausa de 2 segundos
        emitMessage("OK", "Limpieza y reseteo de recursos finalizado.");

    } // Fin del while

    emitMessage("FIN", "Hilo de limpieza ha sido detenido.");
}

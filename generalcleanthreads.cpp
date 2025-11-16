#include "generalcleanthreads.h"
#include <QDebug>
#include <QRandomGenerator>

void GeneralCleanThreads::run() {
    emitMessage("INICIO", "Hilo de limpieza activado y funcionando.");

    while (true) {

        {
            QMutexLocker locker(&mutex);
            if (!isRunning) break;
        }

        // Espera aleatoria entre 15 y 30 segundos
        int delay = QRandomGenerator::global()->bounded(15000, 30001);
        msleep(delay);

        {
            QMutexLocker locker(&mutex);
            if (!isRunning) break;
        }

        // ⛔ NUEVO: No hacer limpieza si la producción está activa
        if (controller && controller->getActiveThreadCount() > 0) {
            emitMessage("INFO", "Limpieza omitida (producción activa).");
            continue;
        }

        // --- LIMPIEZA REAL ---
        emitMessage("ADVERTENCIA", "Realizando reseteo y reactivación de recursos (Simulado).");

        emit requestSystemReset();

        msleep(2000);

        emitMessage("OK", "Limpieza y reseteo de recursos finalizado.");
    }

    emitMessage("FIN", "Hilo de limpieza ha sido detenido.");
}

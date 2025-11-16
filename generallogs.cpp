#include "generallogs.h"
#include <QDebug>
#include <QRandomGenerator>

/**
 * @brief Recopila información detallada del estado de la aplicación.
 * @return Cadena de texto con la bitácora de estado.
 */
QString GeneralLogs::collectLogInformation() const {
    if (!controller) {
        return "ERROR: Controller no disponible.";
    }

    // --- 1. Estado de los Hilos de Producción (Estaciones) ---
    QString log;
    log += "=================================================\n";
    log += "BITACORA DE ESTADO (Production Line)\n";
    log += QString("Hilos de Estación Activos: %1 / %2\n")
               .arg(controller->getActiveThreadCount())
               .arg(controller->getStationList().size());
    log += "-------------------------------------------------\n";

    for (const Station* station : controller->getStationList()) {
        QString status = station->isRunning() ? "ACTIVO" : "INACTIVO";
        QString info = QString("  [ID:%1] %2 (HILO: %3)\n")
                           .arg(station->getId())
                           .arg(station->getName())
                           .arg(status);
        log += info;
    }

    // --- 2. Estado de Recursos (Buffers) ---
    log += "-------------------------------------------------\n";
    log += "ESTADO DE RECURSOS (Buffers)\n";
    log += QString("Buffer 0 (Ensamblaje): %1% de uso\n").arg(controller->getBufferUsage(0));
    log += QString("Buffer 1 (Inspección): %1% de uso\n").arg(controller->getBufferUsage(1));
    log += QString("Buffer 4 (Almacenamiento): %1% de uso\n").arg(controller->getBufferUsage(4));
    log += "=================================================";

    return log;
}

/**
 * @brief Ciclo principal del hilo GeneralLogs.
 * Muestra el estado del sistema periódicamente.
 */
void GeneralLogs::run() {
    emitMessage("INICIO", "Hilo de bitácora activado y monitoreando.");

    while (true) {
        {
            QMutexLocker locker(&mutex);
            if (!isRunning) break;
        }

        // --- SIMULACIÓN DE ACCIÓN DE LOGGING ---
        // 1. Espera de 5 segundos
        msleep(5000);

        {
            QMutexLocker locker(&mutex);
            if (!isRunning) break;
        }

        // 2. Recopilar y enviar el mensaje
        QString info = collectLogInformation();

        // El mensaje completo de la bitácora lo enviamos como la 'message'
        emitMessage("LOG_ACTIVITY", info);
    } // Fin del while

    emitMessage("FIN", "Hilo de bitácora ha sido detenido.");
}

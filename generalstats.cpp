#include "generalstats.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

/**
 * @brief Recopila datos estadísticos clave y los formatea como JSON.
 * @return Cadena JSON con métricas de rendimiento.
 */
QString GeneralStats::collectStatsData() const {
    if (!controller) {
        return "{}";
    }

    QJsonObject stats;

    // Métricas de Rendimiento
    int total = controller->getTotalGoal();
    int completed = controller->getCompletedCount();
    stats["completed_products"] = completed;
    stats["production_goal"] = total;
    stats["active_threads"] = controller->getActiveThreadCount();

    // Métricas de Recursos
    stats["buffer_0_usage"] = controller->getBufferUsage(0);
    stats["buffer_1_usage"] = controller->getBufferUsage(1);  // NUEVO
    stats["buffer_2_usage"] = controller->getBufferUsage(2);
    stats["buffer_3_usage"] = controller->getBufferUsage(3);  // NUEVO
    stats["buffer_4_usage"] = controller->getBufferUsage(4);

    // Persistencia (Asumimos que Logger tiene un método para contar logs)
    // stats["logs_recorded"] = controller->getLogger()->getRecordCount(); // Si existe el método

    QJsonDocument doc(stats);
    return doc.toJson(QJsonDocument::Compact);
}

/**
 * @brief Ciclo principal del hilo GeneralStats.
 * Recopila y emite datos estadísticos periódicamente.
 */
void GeneralStats::run() {
    emitMessage("INICIO", "Hilo de estadísticas activado.");

    while (true) {
        {
            QMutexLocker locker(&mutex);
            if (!isRunning) break;
        }

        // --- SIMULACIÓN DE ACCIÓN DE ESTADÍSTICAS ---
        // 1. Espera de 3 segundos (las estadísticas se actualizan más rápido que la bitácora)
        msleep(3000);

        {
            QMutexLocker locker(&mutex);
            if (!isRunning) break;
        }

        // 2. Recopilar y enviar el mensaje
        QString statsJson = collectStatsData();

        // 3. Emitir las señales requeridas
        emitMessage("DATOS_COLECTADOS", "Estadísticas actualizadas.");
        emit statsDataReady(statsJson);
    } // Fin del while

    emitMessage("FIN", "Hilo de estadísticas ha sido detenido.");
}

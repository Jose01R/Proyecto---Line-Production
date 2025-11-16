#ifndef GENERALSTATS_H
#define GENERALSTATS_H

#include "generalthread.h"
#include "productioncontroller.h"

/**
 * @brief Hilo de mantenimiento GeneralStats.
 * Recopila datos para la visualización gráfica de métricas.
 */
class GeneralStats : public GeneralThread {
    Q_OBJECT

private:
    const ProductionController* controller;

public:
    // Constructor
    GeneralStats(const ProductionController* controller, QObject* parent = nullptr)
        : GeneralThread("GeneralStats", parent), controller(controller) {}

protected:
    void run() override;

private:
    // Método auxiliar para construir un JSON o una cadena de datos estadísticos
    QString collectStatsData() const;

signals:
    // Señal específica para enviar datos estadísticos para el gráfico
    // Podría ser QJsonObject, pero lo simplificamos a QString para este paso.
    void statsDataReady(const QString& data);
};

#endif // GENERALSTATS_H

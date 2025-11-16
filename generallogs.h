#ifndef GENERALLOGS_H
#define GENERALLOGS_H

#include "generalthread.h"
#include "productioncontroller.h" // Necesario para acceder a los datos del sistema

/**
 * @brief Hilo de mantenimiento GeneralLogs.
 * Se encarga de recopilar información de funcionamiento (bitácora de hilos y recursos).
 */
class GeneralLogs : public GeneralThread {
    Q_OBJECT

private:
    // Referencia al controlador para obtener el estado del sistema
    const ProductionController* controller;

public:
    // Constructor
    GeneralLogs(const ProductionController* controller, QObject* parent = nullptr)
        : GeneralThread("GeneralLogs", parent), controller(controller) {}

protected:
    // Implementación del ciclo de vida del hilo
    void run() override;

private:
    // Método auxiliar para construir el mensaje de la bitácora
    QString collectLogInformation() const;
};

#endif // GENERALLOGS_H

#ifndef GENERALCLEANTHREADS_H
#define GENERALCLEANTHREADS_H

#include "generalthread.h"
#include <QTimer> // Usaremos QTimer para un ciclo de limpieza periódico
#include <QCoreApplication> // Para acceder a la lista de objetos

/**
 * @brief Hilo de mantenimiento para limpieza general y reseteo.
 * Lanza periódicamente una acción de 'limpieza' general de recursos.
 */
class GeneralCleanThreads : public GeneralThread {
    Q_OBJECT

public:
    // Constructor
    GeneralCleanThreads(QObject* parent = nullptr)
        : GeneralThread("GeneralCleanThreads", parent) {}

protected:
    // Implementación del ciclo de vida del hilo
    void run() override;

private:
    // Aquí podrías tener una referencia o señal a la clase que gestiona
    // todos los hilos y recursos para hacer la limpieza real.
    // Por ahora, solo simulará y emitirá mensajes.

signals:
    // Señal específica para indicar a la aplicación que debe resetear sus recursos
    void requestSystemReset();
};

#endif // GENERALCLEANTHREADS_H

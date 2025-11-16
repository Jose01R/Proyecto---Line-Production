#ifndef STATION_H
#define STATION_H

#include <QThread>
#include <QString>
#include <QDebug>
#include "buffer.h"
#include "product.h"

/**
 * Clase que representa una estación de trabajo (hilo) en la línea de producción.
 * Hereda de QThread para ejecutar la lógica de procesamiento en un hilo separado.
 */
class Station : public QThread {
    Q_OBJECT

protected:
    int id;              // ID de la estación
    QString name;        // Nombre de la estación
    QString taskType;    // Tipo de tarea
    Buffer* inputBuffer; // Buffer de entrada
    Buffer* outputBuffer;// Buffer de salida
    volatile bool running; // Control de ejecución
    QString lastStatus;  // Último estado enviado (para evitar spam)

public:
    Station(int id, const QString& name, const QString& taskType,
            Buffer* input, Buffer* output, QObject* parent = nullptr);

    ~Station() override;

    // Lógica específica por estación (Assembler, Tester, etc.)
    virtual void processProduct(Product& product) = 0;

    // Hilo
    void run() override;

    // Detener estación sin congelar GUI
    void stopStation();

    // Getters
    QString getName() const { return name; }
    int getId() const { return id; }

signals:
    // Estado de la estación (para GUI)
    void stationStatusUpdate(int stationId, const QString& status);

    // Producto completado en esta estación
     void productFinishedProcessing(Product* product, const QString& stationName);
};

#endif // STATION_H


#ifndef STATION_H
#define STATION_H

#include <QThread>
#include <QString>
#include <QDebug>
#include "buffer.h"
#include "product.h"

// Forward declaration para evitar ciclos
class ProductionController;

/**
 * Clase que representa una estación de trabajo (hilo) en la línea de producción.
 */
class Station : public QThread {
    Q_OBJECT

protected:
    int id;
    QString name;
    QString taskType;
    Buffer* inputBuffer;
    Buffer* outputBuffer;
    volatile bool running;
    QString lastStatus;

    ProductionController* controller;   // NECESARIO PARA PAUSA/REANUDAR

public:
    Station(int id,
            const QString& name,
            const QString& taskType,
            Buffer* input, Buffer* output,
            ProductionController* controller,
            QObject* parent = nullptr);

    ~Station() override;

    virtual void processProduct(Product& product) = 0;

    void run() override;

    void stopStation();

    QString getName() const { return name; }
    int getId() const { return id; }

signals:
    void stationStatusUpdate(int stationId, const QString& status);
    void productFinishedProcessing(Product* product, const QString& stationName);
};

#endif // STATION_H

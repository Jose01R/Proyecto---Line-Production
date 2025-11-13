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
    Q_OBJECT //para la comunicación Signals/Slots

protected:
    int id; //ID de la estación
    QString name; // Nombre de la estación
    QString taskType; //Tipo tarea que realiza (Ensamblar Pieza, Inspeccionar Calidad)
    Buffer* inputBuffer; // Puntero al buffer de entrada de la estación
    Buffer* outputBuffer; // Puntero al buffer de salida de la estación
    volatile bool running; // Bandera para controlar ejecución del hilo de forma segura
    QString lastStatus;

public:
    // Constructor
    Station(int id, const QString& name, const QString& taskType, Buffer* input, Buffer* output, QObject* parent = nullptr);

    // Destructor --> destrucción de clases derivadas
    ~Station() override;

    // Método virtual puro: la lógica específica de procesamiento debe ser implementada por las clases derivadas
    virtual void processProduct(Product& product) = 0;

    //punto de entrada para la ejecución del hilo
    void run() override;
    //detener el hilo de la estación
    void stopStation();

    // Getters
    QString getName() const { return name; }
    int getId() const { return id; }

signals:
    // Señal para actualizar la GUI con el estado de la estación
    void stationStatusUpdate(int stationId, const QString& status);
    // Señal para notificar que un producto ha terminado de ser procesado por esta estación
    void productFinishedProcessing(const Product& product, const QString& stationName);
};

#endif // STATION_H

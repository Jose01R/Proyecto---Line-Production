#ifndef STATION_H
#define STATION_H

#include <QThread>
#include <QString>
#include <QDebug>
#include "buffer.h" // Incluye la definición de Buffer
#include "product.h" // Incluye la definición de Product

/**
 * @brief Clase base que representa una estación de trabajo (hilo) en la línea de producción.
 * Hereda de QThread para ejecutar la lógica de procesamiento en un hilo separado.
 */
class Station : public QThread {
    Q_OBJECT // Necesario para la comunicación Signals/Slots de Qt

protected:
    int id; // Identificador único de la estación
    QString name; // Nombre de la estación (ej: "Ensamblaje", "Inspección")
    QString taskType; // Tipo de tarea que realiza (ej: "Ensamblar Pieza", "Inspeccionar Calidad")
    Buffer* inputBuffer; // Puntero al buffer de entrada de la estación
    Buffer* outputBuffer; // Puntero al buffer de salida de la estación
    volatile bool running; // Bandera para controlar la ejecución del hilo de forma segura
    QString lastStatus;

public:
    // Constructor
    Station(int id, const QString& name, const QString& taskType, Buffer* input, Buffer* output, QObject* parent = nullptr);

    // Destructor para asegurar la correcta destrucción de clases derivadas
    ~Station() override;

    // Método virtual puro: la lógica específica de procesamiento debe ser implementada por las clases derivadas
    virtual void processProduct(Product& product) = 0;

    // El método run() es el punto de entrada para la ejecución del hilo
    void run() override;
    // Método para detener el hilo de la estación de forma segura
    void stopStation();

    // Getters para acceder a propiedades de la estación
    QString getName() const { return name; }
    int getId() const { return id; }

signals:
    // Señal para actualizar la interfaz gráfica con el estado de la estación
    void stationStatusUpdate(int stationId, const QString& status);
    // Señal para notificar que un producto ha terminado de ser procesado por esta estación
    void productFinishedProcessing(const Product& product, const QString& stationName);
};

#endif // STATION_H

#ifndef PRODUCTIONCONTROLLER_H
#define PRODUCTIONCONTROLLER_H

#include <QObject>
#include <QList>
#include <QDebug>
#include <QTimer> // Para generar productos a intervalos regulares
#include "station.h" // Incluye la clase base Station
#include "buffer.h"  // Incluye la clase Buffer
#include "assembler.h" // Incluye la clase Assembler
#include "tester.h"    // Incluye la clase Tester
#include "product.h"   // Incluye la clase Product

/**
 * @brief Clase que controla toda la línea de producción: crea estaciones, buffers y gestiona el flujo.
 */
class ProductionController : public QObject {
    Q_OBJECT

private:
    QList<Station*> stationList; // Lista de todas las estaciones en la línea
    QList<Buffer*> bufferList; // Lista de buffers que conectan las estaciones
    int productIdCounter; // Contador para asignar IDs únicos a los productos
    QTimer* productGenerationTimer; // Temporizador para generar productos

public:
    explicit ProductionController(QObject* parent = nullptr);
    ~ProductionController() override;

    void setupProductionLine(int numberOfStations = 2); // Configura la línea de producción

public slots:
    void startProduction(); // Inicia todos los hilos de las estaciones
    void stopProduction();  // Detiene todos los hilos de las estaciones
    void generateProduct(); // Slot para generar nuevos productos

signals:
    // Señales para la GUI o un logger centralizado
    void newProductCreated(const Product& product);
    void productionLineStatus(const QString& status);
    // Señal para enviar el estado de una estación directamente (podría conectarse a la señal de Station)
    void updateStationStatus(int stationId, const QString& status);
    void productStateChanged(const Product& product, const QString& stationName);
};

#endif // PRODUCTIONCONTROLLER_H

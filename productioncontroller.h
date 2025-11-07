#ifndef PRODUCTIONCONTROLLER_H
#define PRODUCTIONCONTROLLER_H

#include <QObject>
#include <QList>
#include <QDebug>
#include <QTimer>
#include <QFile>         // Añadido para manejo de archivos
#include <QJsonDocument> // Añadido para JSON
#include <QJsonObject>   // Añadido para JSON
#include <QJsonArray>    // Añadido para JSON

// Incluye todas las clases necesarias
#include "station.h"
#include "buffer.h"
#include "assembler.h"
#include "tester.h"
#include "product.h"

/**
 * @brief Clase que controla toda la línea de producción y gestiona la persistencia JSON.
 */
class ProductionController : public QObject {
    Q_OBJECT

private:
    QList<Station*> stationList;
    QList<Buffer*> bufferList;
    int productIdCounter;
    QTimer* productGenerationTimer;
    const QString DATA_FILENAME = "production_data.json"; // Archivo de persistencia

public:
    explicit ProductionController(QObject* parent = nullptr);
    ~ProductionController() override;

    void setupProductionLine(int numberOfStations = 2);

    // --- MÉTODOS DE PERSISTENCIA JSON ---
    bool saveDataToJson() const;
    bool loadDataFromJson();
    // ------------------------------------

public slots:
    void startProduction();
    void stopProduction();
    void generateProduct();

signals:
    void newProductCreated(const Product& product);
    void productionLineStatus(const QString& status);
    void updateStationStatus(int stationId, const QString& status);
    void productStateChanged(const Product& product, const QString& stationName);
};

#endif // PRODUCTIONCONTROLLER_H

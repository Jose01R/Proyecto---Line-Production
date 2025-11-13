#ifndef PRODUCTIONCONTROLLER_H
#define PRODUCTIONCONTROLLER_H

#include <QObject>
#include <QList>
#include <QTimer>
#include "station.h"
#include "buffer.h"
#include "product.h"

// Inclusiones de las clases
#include "assembler.h"
#include "tester.h"
#include "labeler.h"
#include "packager.h"
#include "storage.h"
#include "applianceGenerator.h"
#include "logger.h"

class ProductionController : public QObject {
    Q_OBJECT

private:
    QList<Station*> stationList;
    QList<Buffer*> bufferList;
    QTimer* productGenerationTimer;

    // Ajustado el orden para coincidir con la lista de inicialización del constructor (para -Wreorder)
    int totalGoal;
    int completedCount;

    // El logger y el generator deben ir después si se inicializan con miembros previos
    Logger *logger;
    ApplianceGenerator generator;

    int productIdCounter;

public:
    explicit ProductionController(QObject* parent = nullptr);
    ~ProductionController() override;

    void setupProductionLine(int numberOfStations = 5);
    void setProductionGoal(int amount);

    int getTotalGoal() const { return totalGoal; }
    int getCompletedCount() const { return completedCount; }

    // NUEVO GETTER: Permite a MainWindow acceder a la lista para el log visual.
    const QList<Station*>& getStationList() const { return stationList; }

    Logger* getLogger() const { return logger; }

    int getActiveThreadCount() const;
    int getBufferUsage(int index) const;

public slots:
    void startProduction();
    void stopProduction();
    void generateProduct();
    void onFinalProductFinished(const Product& product, const QString& stationName);

signals:
    void newProductCreated(const Product& product);
    void productionLineStatus(const QString& status);
    void updateStationStatus(int stationId, const QString& status);
    void productStateChanged(const Product& product, const QString& stationName);
};

#endif // PRODUCTIONCONTROLLER_H

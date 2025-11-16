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

    int totalGoal;
    int completedCount;


    Logger *logger;
    ApplianceGenerator generator;

    int productIdCounter;
    bool isPaused = false;
public:
    explicit ProductionController(QObject* parent = nullptr);
    ~ProductionController() override;

    void setupProductionLine(int numberOfStations = 5);
    void setProductionGoal(int amount);

    int getTotalGoal() const { return totalGoal; }
    int getCompletedCount() const { return completedCount; }


    const QList<Station*>& getStationList() const { return stationList; }

    Logger* getLogger() const { return logger; }

    int getActiveThreadCount() const;
    int getBufferUsage(int index) const;

    bool getPaused() const { return isPaused; }
public slots:
    void startProduction();
    void stopProduction();
    void generateProduct();
    // Cambiamos la firma: ahora recibe Product*
    void onFinalProductFinished(Product* product, const QString& stationName);

    // Slot para estaciones intermedias: solo re-emite estado a la GUI sin borrar
    void onIntermediateProductFinished(Product* product, const QString& stationName);


    void pauseProduction();
    void resumeProduction();
signals:
    void newProductCreated(const Product& product);
    void productionLineStatus(const QString& status);
    void updateStationStatus(int stationId, const QString& status);
    void productStateChanged(const Product& product, const QString& stationName);
};

#endif // PRODUCTIONCONTROLLER_H

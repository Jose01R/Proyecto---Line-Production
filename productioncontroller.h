#ifndef PRODUCTIONCONTROLLER_H
#define PRODUCTIONCONTROLLER_H

#include <QObject>
#include <QList>
#include <QTimer>
#include "station.h"
#include "buffer.h"
#include "assembler.h"
#include "tester.h"
#include "product.h"

class ProductionController : public QObject {
    Q_OBJECT

private:
    QList<Station*> stationList;
    QList<Buffer*> bufferList;
    QTimer* productGenerationTimer;

    int productIdCounter;
    int totalGoal;
    int completedCount;

public:
    explicit ProductionController(QObject* parent = nullptr);
    ~ProductionController() override;

    void setupProductionLine(int numberOfStations = 5);
    void setProductionGoal(int amount);

    int getTotalGoal() const { return totalGoal; }
    int getCompletedCount() const { return completedCount; }

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


#include "productioncontroller.h"
#include <QDebug>

ProductionController::ProductionController(QObject* parent)
    : QObject(parent),
    productIdCounter(0),
    totalGoal(0),
    completedCount(0)
{
    productGenerationTimer = new QTimer(this);
    connect(productGenerationTimer, &QTimer::timeout, this, &ProductionController::generateProduct);

    qDebug() << "ProductionController creado.";
}

ProductionController::~ProductionController() {
    stopProduction();
    qDeleteAll(stationList);
    qDeleteAll(bufferList);
}

void ProductionController::setProductionGoal(int amount) {
    totalGoal = amount;
    completedCount = 0;
    qDebug() << "Meta de producción establecida:" << totalGoal;
}

void ProductionController::setupProductionLine(int numberOfStations) {

    if (numberOfStations < 1) return;

    bufferList.clear();
    stationList.clear();

    for (int i = 0; i < numberOfStations + 1; ++i) {
        bufferList.append(new Buffer());
        qDebug() << "Buffer" << i << "creado.";
    }

    Assembler* assembler = new Assembler(1, bufferList[0], bufferList[1], this);
    stationList.append(assembler);
    connect(assembler, &Station::stationStatusUpdate, this, &ProductionController::updateStationStatus);
    connect(assembler, &Station::productFinishedProcessing, this, &ProductionController::productStateChanged);

    Tester* tester1 = new Tester(2, bufferList[1], bufferList[2], this);
    stationList.append(tester1);
    connect(tester1, &Station::stationStatusUpdate, this, &ProductionController::updateStationStatus);
    connect(tester1, &Station::productFinishedProcessing, this, &ProductionController::productStateChanged);

    Tester* tester2 = new Tester(3, bufferList[2], bufferList[3], this);
    stationList.append(tester2);
    connect(tester2, &Station::stationStatusUpdate, this, &ProductionController::updateStationStatus);
    connect(tester2, &Station::productFinishedProcessing, this, &ProductionController::productStateChanged);

    Tester* tester3 = new Tester(4, bufferList[3], bufferList[4], this);
    stationList.append(tester3);
    connect(tester3, &Station::stationStatusUpdate, this, &ProductionController::updateStationStatus);
    connect(tester3, &Station::productFinishedProcessing, this, &ProductionController::productStateChanged);

    Tester* storage = new Tester(5, bufferList[4], nullptr, this);
    stationList.append(storage);
    connect(storage, &Station::stationStatusUpdate, this, &ProductionController::updateStationStatus);
    connect(storage, &Station::productFinishedProcessing, this, &ProductionController::onFinalProductFinished);

    emit productionLineStatus("Línea configurada.");
}

void ProductionController::startProduction() {

    if (totalGoal <= 0) {
        emit productionLineStatus("Debe establecer meta antes de iniciar");
        return;
    }

    productGenerationTimer->start(1000);
    emit productionLineStatus("Producción iniciada.");

    for (Station* station : stationList) {
        if (!station->isRunning()) {
            station->start();
        }
    }
}

void ProductionController::stopProduction() {
    productGenerationTimer->stop();
    emit productionLineStatus("Producción detenida.");

    for (Station* station : stationList) {
        station->stopStation();
    }
}

void ProductionController::generateProduct() {

    if (completedCount >= totalGoal) {
        stopProduction();
        return;
    }

    Product* newProduct = new Product(productIdCounter++, "Electrodoméstico");
    qDebug() << "Generando producto" << newProduct->showInfo();

    if (!bufferList[0]->tryAddProduct(newProduct, 50)) {
        qDebug() << "Buffer 0 lleno; reintento en el próximo tick";
        delete newProduct;                  // o guárdalo para reintentar, como prefieras
        return;
    }

    emit newProductCreated(*newProduct);
}

void ProductionController::onFinalProductFinished(const Product& product, const QString& stationName) {

    completedCount++;

    emit productStateChanged(product, stationName);

    qDebug() << "Producto finalizado:" << completedCount << "/" << totalGoal;

    if (completedCount >= totalGoal) {
        stopProduction();
        emit productionLineStatus("Meta alcanzada");
    }
}

int ProductionController::getActiveThreadCount() const {
    int active = 0;
    for (auto station : stationList) {
        if (station->isRunning()) active++;
    }
    return active;
}

int ProductionController::getBufferUsage(int index) const {
    if (index < 0 || index >= bufferList.size())
        return 0;

    Buffer* b = bufferList[index];
    return (b->size() * 100) / b->getCapacity();
}

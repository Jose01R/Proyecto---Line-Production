#include "productioncontroller.h"
#include "applianceGenerator.h" // Clase para generar productos
#include "logger.h"             // Clase para persistencia JSON
// Incluir todas las estaciones usadas:
#include "assembler.h"
#include "tester.h"
#include "labeler.h"
#include "packager.h"
#include "storage.h"

ProductionController::ProductionController(QObject* parent)
    : QObject(parent),
    productIdCounter(0),
    totalGoal(0),
    completedCount(0),
    logger(new Logger(this)) // Inicializa el Logger (importante)
{
    productGenerationTimer = new QTimer(this);
    connect(productGenerationTimer, &QTimer::timeout, this, &ProductionController::generateProduct);

    qDebug() << "ProductionController creado.";
}

ProductionController::~ProductionController() {
    //Asegura la detención segura de hilos antes de liberar memoria
    stopProduction();

    // Limpieza de memoria: eliminar cualquier producto restante en los buffers
    for (Buffer* buffer : bufferList) {
        while (buffer->size() > 0) {
            // Asume que removeProduct() está disponible y devuelve un puntero
            Product* remainingProduct = buffer->removeProduct();
            if (remainingProduct) {
                delete remainingProduct;
            }
        }
    }

    qDeleteAll(stationList);
    qDeleteAll(bufferList);
}

void ProductionController::setProductionGoal(int amount) {
    totalGoal = amount;
    completedCount = 0;
    qDebug() << "Meta de producción establecida:" << totalGoal;
}

void ProductionController::setupProductionLine(int numberOfStations) {

    const int NUM_STATIONS = 5; // Hardcodeamos a 5 estaciones (Ass, Test, Label, Pack, Store)

    stopProduction();
    qDeleteAll(stationList);
    qDeleteAll(bufferList);
    stationList.clear();
    bufferList.clear();

    //Creación de Buffers (Se necesitan 5 buffers para 5 estaciones)
    for (int i = 0; i < NUM_STATIONS; ++i) {
        bufferList.append(new Buffer());
    }

    //Creación de Estaciones: B0 -> S1(Ass) -> B1 -> S2(Test) -> B2 -> S3(Label) -> B3 -> S4(Pack) -> B4 -> S5(Store)

    Assembler* s1 = new Assembler(1, bufferList[0], bufferList[1], this);
    Tester* s2 = new Tester(2, bufferList[1], bufferList[2], this);
    Labeler* s3 = new Labeler(3, bufferList[2], bufferList[3], this);
    Packager* s4 = new Packager(4, bufferList[3], bufferList[4], this);
    Storage* s5 = new Storage(5, bufferList[4], nullptr, this); // S5 es Storage y termina la línea

    stationList.append({s1, s2, s3, s4, s5});

    //Conexiones de Señales
    for (Station* station : stationList) {
        connect(station, &Station::stationStatusUpdate, this, &ProductionController::updateStationStatus);
        connect(station, &Station::productFinishedProcessing, this, &ProductionController::productStateChanged);

        // Conexión específica para última estación (Storage)
        if (station->getId() == NUM_STATIONS) {
            connect(station, &Station::productFinishedProcessing, this, &ProductionController::onFinalProductFinished);
        }
    }

    emit productionLineStatus("Línea configurada con 5 estaciones y 5 buffers.");
}

void ProductionController::startProduction() {
    if (totalGoal <= 0) {
        emit productionLineStatus("Debe establecer meta antes de iniciar");
        return;
    }

    productGenerationTimer->start(1000); // Genera un producto cada 1s
    emit productionLineStatus("Producción iniciada.");

    for (Station* station : stationList) {
        if (!station->isRunning()) {
            station->start();
        }
    }
}

void ProductionController::stopProduction() {
    productGenerationTimer->stop();
    emit productionLineStatus("Producción detenida");

    for (Station* station : stationList) {
        station->stopStation();
    }
}

void ProductionController::generateProduct() {

    if (completedCount >= totalGoal) {
        productGenerationTimer->stop();
        return;
    }

    // Usa el ApplianceGenerator para crear el producto (Product ID ahora lo gestiona el Generator)
    Product* newProduct = generator.generateNewProduct();

    if (!newProduct) {
        qWarning() << "Error al generar producto.";
        return;
    }

    // Intenta añadir al primer buffer (Buffer 0) con un pequeño timeout
    if (!bufferList[0]->tryAddProduct(newProduct, 50)) {
        qDebug() << "Buffer 0 lleno; producto descartado temporalmente. ID:" << newProduct->getId();
        // ¡CRÍTICO! Si no pudo entrar al buffer, debe ser eliminado aquí.
        delete newProduct;
        return;
    }

    emit newProductCreated(*newProduct);
}

void ProductionController::onFinalProductFinished(const Product& product, const QString& stationName) {

    completedCount++;

    // Registrar en el Logger
    logger->recordCompletion(product);

    //Notificar a la GUI
    emit productStateChanged(product, stationName);

    qDebug() << "Producto finalizado:" << completedCount << "/" << totalGoal;

    if (completedCount >= totalGoal) {
        stopProduction();
        emit productionLineStatus("Meta alcanzada. Deteniendo producción.");
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
    int capacity = b->getCapacity();
    return (capacity > 0) ? (b->size() * 100) / capacity : 0;
}

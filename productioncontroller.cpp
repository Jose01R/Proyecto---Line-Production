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
    qRegisterMetaType<Product*>("Product*"); // <- IMPORTANTE (para queued signals)
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

    const int NUM_STATIONS = 5;

    stopProduction();
    qDeleteAll(stationList);
    qDeleteAll(bufferList);
    stationList.clear();
    bufferList.clear();

    for (int i = 0; i < NUM_STATIONS; ++i) {
        bufferList.append(new Buffer());
    }
    Assembler* s1 = new Assembler(1, bufferList[0], bufferList[1], this, this);
    Tester*    s2 = new Tester(2, bufferList[1], bufferList[2], this, this);
    Labeler*   s3 = new Labeler(3, bufferList[2], bufferList[3], this, this);
    Packager*  s4 = new Packager(4, bufferList[3], bufferList[4], this, this);
    Storage*   s5 = new Storage(5, bufferList[4], this, this);



    stationList.append({s1, s2, s3, s4, s5});

    for (Station* station : stationList) {
        connect(station, &Station::stationStatusUpdate,
                this, &ProductionController::updateStationStatus);

        if (station->getId() == NUM_STATIONS) {
            // Solo Storage dispara final -> manejador que BORRA el Product*
            connect(station, &Station::productFinishedProcessing,
                    this, &ProductionController::onFinalProductFinished);
        } else {
            // Estaciones 1–4 -> manejador intermedio que solo notifica GUI
            connect(station, &Station::productFinishedProcessing,
                    this, &ProductionController::onIntermediateProductFinished);
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
    emit productionLineStatus("Producción detenida.");

    // Despertar buffers para que los hilos salgan del bloqueo
    for (Buffer* buffer : bufferList) {
        buffer->forceWake();
    }

    // Pedimos a las estaciones que paren
    for (Station* station : stationList) {
        station->stopStation();
    }

    // Esperamos a que terminen (timeout razonable)
    for (Station* station : stationList) {
        if (station->isRunning()) {
            if (!station->wait(2000)) { // 2s
                qWarning() << "Station" << station->getName() << "no terminó en 2s tras stopProduction().";
                // Como último recurso no llamamos terminate() automáticamente,
                // podrías considerar station->terminate() si es imprescindible (no recomendado).
            }
        }
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

void ProductionController::onIntermediateProductFinished(Product* product, const QString& stationName) {
    if (!product) return;
    // Re-emito la señal que ya tenías para la GUI pero con referencia (no borrar)
    emit productStateChanged(*product, stationName);
    // NO borrar el product: la estación anterior o el buffer lo seguirá gestionando.
}

void ProductionController::onFinalProductFinished(Product* product, const QString& stationName) {
    if (!product) return;

    completedCount++;

    // Registrar en el Logger (Logger usa copia de datos)
    logger->recordCompletion(*product);

    //Notificar a la GUI (usando referencia temporal)
    emit productStateChanged(*product, stationName);

    qDebug() << "Producto finalizado:" << completedCount << "/" << totalGoal;

    // Ahora BORRAMOS el producto (porque es el final)
    delete product;

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
void ProductionController::pauseProduction() {
    isPaused = true;
    emit productionLineStatus("Producción pausada.");
}

void ProductionController::resumeProduction() {
    isPaused = false;
    emit productionLineStatus("Producción reanudada.");
}


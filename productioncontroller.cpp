#include "productioncontroller.h"

ProductionController::ProductionController(QObject* parent)
    : QObject(parent), productIdCounter(0) {

    // Configura el temporizador para generar productos
    productGenerationTimer = new QTimer(this);
    // Conecta el timeout del temporizador a la función generateProduct()
    connect(productGenerationTimer, &QTimer::timeout, this, &ProductionController::generateProduct);

    qDebug() << "ProductionController creado.";
}

ProductionController::~ProductionController() {
    stopProduction(); // Asegura que los hilos se detengan antes de eliminar objetos

    // Libera la memoria de las estaciones y buffers
    qDeleteAll(stationList);
    qDeleteAll(bufferList);

    qDebug() << "ProductionController destruido.";
}

void ProductionController::setupProductionLine(int numberOfStations) {
    if (numberOfStations < 1) {
        qWarning() << "El número de estaciones debe ser al menos 1.";
        return;
    }

    // Crear buffers: Necesitas (numberOfStations + 1) buffers para conectar todas las estaciones y un inicio/fin.
    // Buffer 0: Entrada de productos (para el generador)
    // Buffer 1 a (numberOfStations-1): Buffers intermedios
    // Buffer numberOfStations: Salida final de productos
    for (int i = 0; i < numberOfStations + 1; ++i) {
        bufferList.append(new Buffer());
        qDebug() << "Buffer " << i << " creado.";
    }

    // Crear estaciones y conectarlas con los buffers
    // Asumiremos una línea simple: Ensamblaje -> Inspección -> ...
    // Puedes personalizar esto para una línea más compleja (ej: múltiples ensambladores)

    // Primera estación: Ensamblador
    Assembler* assembler = new Assembler(1, bufferList[0], bufferList[1], this);
    stationList.append(assembler);
    // Conectar señales de la estación al controlador (para re-emitir o procesar)
    connect(assembler, &Station::stationStatusUpdate, this, &ProductionController::updateStationStatus);
    connect(assembler, &Station::productFinishedProcessing, this, &ProductionController::productStateChanged);
    qDebug() << "Estación 1: Ensamblaje agregada.";

    // Estaciones intermedias (si hay más de 2 estaciones en total)
    for (int i = 2; i <= numberOfStations; ++i) {
        // Aquí podrías alternar entre Tester, otros tipos de estaciones, etc.
        // Por simplicidad, añadiremos un Tester.
        Tester* tester = new Tester(i, bufferList[i-1], bufferList[i], this);
        stationList.append(tester);
        connect(tester, &Station::stationStatusUpdate, this, &ProductionController::updateStationStatus);
        connect(tester, &Station::productFinishedProcessing, this, &ProductionController::productStateChanged);
        qDebug() << "Estación " << i << ": Inspección agregada.";
    }

    emit productionLineStatus("Línea de producción configurada con " + QString::number(stationList.size()) + " estaciones.");
}

void ProductionController::startProduction() {
    qDebug() << "Iniciando producción...";
    emit productionLineStatus("Producción Iniciada");

    // Iniciar el temporizador para la generación de productos (ej: cada 2 segundos)
    productGenerationTimer->start(2000);

    // Iniciar todos los hilos de las estaciones
    for (Station* station : stationList) {
        if (!station->isRunning()) {
            station->start(); // Inicia el hilo de la estación
            qDebug() << "Hilo de estación " << station->getName() << " iniciado.";
        }
    }
}

void ProductionController::stopProduction() {
    qDebug() << "Deteniendo producción...";
    emit productionLineStatus("Producción Detenida");

    // Detener el temporizador de generación de productos
    if (productGenerationTimer->isActive()) {
        productGenerationTimer->stop();
    }

    // Detener todos los hilos de las estaciones
    for (Station* station : stationList) {
        if (station->isRunning()) {
            station->stopStation(); // Solicita la detención segura del hilo
            qDebug() << "Hilo de estación " << station->getName() << " detenido.";
        }
    }
}

void ProductionController::generateProduct() {
    productIdCounter++;
    Product newProduct(productIdCounter, "Electrodoméstico Genérico");
    qDebug() << "Generando nuevo producto:" << newProduct.showInfo();

    // El primer buffer (bufferList[0]) actúa como la entrada de la línea
    bufferList[0]->addProduct(newProduct);

    emit newProductCreated(newProduct);
}

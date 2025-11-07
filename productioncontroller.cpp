#include "productioncontroller.h"
#include <QFileInfo>

ProductionController::ProductionController(QObject* parent)
    : QObject(parent), productIdCounter(0) {

    productGenerationTimer = new QTimer(this);
    connect(productGenerationTimer, &QTimer::timeout, this, &ProductionController::generateProduct);

    qDebug() << "ProductionController creado.";
}

ProductionController::~ProductionController() {
    stopProduction();
    qDeleteAll(stationList);
    qDeleteAll(bufferList);
    qDebug() << "ProductionController destruido.";
}

void ProductionController::setupProductionLine(int numberOfStations) {
    if (numberOfStations < 1) {
        qWarning() << "El número de estaciones debe ser al menos 1.";
        return;
    }

    // Limpiar si ya había algo
    stopProduction();
    qDeleteAll(stationList);
    qDeleteAll(bufferList);
    stationList.clear();
    bufferList.clear();

    // Crear buffers: (numberOfStations + 1)
    for (int i = 0; i < numberOfStations + 1; ++i) {
        bufferList.append(new Buffer());
        qDebug() << "Buffer " << i << " creado.";
    }

    // Crear estaciones y conectarlas
    // Estación 1: Assembler (Entrada: Buffer[0], Salida: Buffer[1])
    Assembler* assembler = new Assembler(1, bufferList[0], bufferList[1], this);
    stationList.append(assembler);
    // Conexiones de señal
    connect(assembler, &Station::stationStatusUpdate, this, &ProductionController::updateStationStatus);
    connect(assembler, &Station::productFinishedProcessing, this, &ProductionController::productStateChanged);
    qDebug() << "Estación 1: Ensamblaje agregada.";

    // Estaciones intermedias (i=2 hasta numberOfStations)
    for (int i = 2; i <= numberOfStations; ++i) {
        Buffer* input = bufferList[i-1];
        Buffer* output = bufferList[i];

        // Alternar tipos de estación (ejemplo)
        Station* newStation = nullptr;
        if (i % 2 == 0) {
            newStation = new Tester(i, input, output, this);
        } else {
            newStation = new Assembler(i, input, output, this);
        }

        stationList.append(newStation);
        connect(newStation, &Station::stationStatusUpdate, this, &ProductionController::updateStationStatus);
        connect(newStation, &Station::productFinishedProcessing, this, &ProductionController::productStateChanged);
        qDebug() << "Estación " << i << ": " << newStation->getName() << " agregada.";
    }

    emit productionLineStatus("Línea de producción configurada con " + QString::number(stationList.size()) + " estaciones.");
}

void ProductionController::startProduction() {
    // ... (El código de startProduction se mantiene igual)
    qDebug() << "Iniciando producción...";
    emit productionLineStatus("Producción Iniciada");

    productGenerationTimer->start(2000);

    for (Station* station : stationList) {
        if (!station->isRunning()) {
            station->start();
            qDebug() << "Hilo de estación " << station->getName() << " iniciado.";
        }
    }
}

void ProductionController::stopProduction() {
    // ... (El código de stopProduction se mantiene igual, ahora usando el stopStation seguro)
    qDebug() << "Deteniendo producción...";
    emit productionLineStatus("Producción Detenida");

    if (productGenerationTimer->isActive()) {
        productGenerationTimer->stop();
    }

    for (Station* station : stationList) {
        if (station->isRunning()) {
            station->stopStation();
            qDebug() << "Hilo de estación " << station->getName() << " detenido.";
        }
    }
}

void ProductionController::generateProduct() {
    // ... (El código de generateProduct se mantiene igual)
    productIdCounter++;
    Product newProduct(productIdCounter, "Electrodoméstico Genérico");
    qDebug() << "Generando nuevo producto:" << newProduct.showInfo();

    bufferList[0]->addProduct(newProduct);

    emit newProductCreated(newProduct);
}


// --- IMPLEMENTACIÓN DE PERSISTENCIA JSON ---

bool ProductionController::saveDataToJson() const {
    qDebug() << "Intentando guardar datos de persistencia...";

    QJsonObject rootObject;

    // 1. Serializar Buffers
    QJsonArray bufferArray;
    for (const Buffer* buffer : bufferList) {
        bufferArray.append(buffer->toJson());
    }
    rootObject["buffers"] = bufferArray;

    // 2. Serializar Estaciones
    QJsonArray stationArray;
    for (const Station* station : stationList) {
        stationArray.append(station->toJson());
    }
    rootObject["stations"] = stationArray;

    // 3. Serializar Contador de Productos
    rootObject["productIdCounter"] = productIdCounter;

    // 4. Escribir en el archivo
    QJsonDocument saveDoc(rootObject);
    QFile saveFile(DATA_FILENAME);

    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Error: No se pudo abrir el archivo para guardar:" << DATA_FILENAME;
        return false;
    }

    saveFile.write(saveDoc.toJson(QJsonDocument::Indented));
    saveFile.close();
    qDebug() << "Éxito: Datos de persistencia guardados en:" << DATA_FILENAME;
    return true;
}

bool ProductionController::loadDataFromJson() {
    QFile loadFile(DATA_FILENAME);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Advertencia: Archivo de datos no encontrado. Iniciando sin cargar datos.";
        return false;
    }

    QByteArray savedData = loadFile.readAll();
    loadFile.close();
    QJsonDocument loadDoc = QJsonDocument::fromJson(savedData);
    QJsonObject rootObject = loadDoc.object();

    // 1. Limpiar estado actual (es CRÍTICO antes de cargar)
    stopProduction();
    qDeleteAll(stationList);
    qDeleteAll(bufferList);
    stationList.clear();
    bufferList.clear();

    // 2. Cargar Buffers
    if (rootObject.contains("buffers") && rootObject["buffers"].isArray()) {
        QJsonArray bufferArray = rootObject["buffers"].toArray();
        for (const QJsonValue& value : bufferArray) {
            if (value.isObject()) {
                Buffer* newBuffer = new Buffer();
                newBuffer->fromJson(value.toObject());
                bufferList.append(newBuffer);
            }
        }
    } else {
        qWarning() << "Error: Datos de buffers faltantes o inválidos.";
        return false;
    }

    // 3. Cargar Estaciones
    if (rootObject.contains("stations") && rootObject["stations"].isArray()) {
        QJsonArray stationArray = rootObject["stations"].toArray();
        for (int i = 0; i < stationArray.size(); ++i) {
            const QJsonValue& value = stationArray.at(i);
            if (value.isObject()) {
                QJsonObject stationJson = value.toObject();
                QString className = stationJson["className"].toString();

                Station* newStation = nullptr;

                // Asignar Buffers basados en la posición (Asume un flujo lineal y completo)
                if (i < bufferList.size() - 1) {
                    Buffer* input = bufferList[i];
                    Buffer* output = bufferList[i+1];

                    // Re-crear la instancia de la clase correcta
                    if (className == "Assembler") {
                        // Usamos un ID temporal (0) ya que fromJson restaurará el ID real
                        newStation = new Assembler(0, input, output, this);
                    } else if (className == "Tester") {
                        newStation = new Tester(0, input, output, this);
                    }

                    if (newStation) {
                        newStation->fromJson(stationJson);
                        stationList.append(newStation);
                        // Reconectar señales
                        connect(newStation, &Station::stationStatusUpdate, this, &ProductionController::updateStationStatus);
                        connect(newStation, &Station::productFinishedProcessing, this, &ProductionController::productStateChanged);
                    } else {
                        qWarning() << "Error: Tipo de estación desconocido:" << className;
                    }
                } else {
                    qWarning() << "Error: No hay suficientes buffers para enlazar la estación cargada. Deteniendo carga de estaciones.";
                    break;
                }
            }
        }
    } else {
        qWarning() << "Error: Datos de estaciones faltantes o inválidos.";
        return false;
    }

    // 4. Cargar Contador de Productos
    if (rootObject.contains("productIdCounter") && rootObject["productIdCounter"].isDouble()) {
        productIdCounter = rootObject["productIdCounter"].toInt();
        qDebug() << "Contador de productos restaurado a:" << productIdCounter;
    }

    emit productionLineStatus("Línea de producción restaurada desde archivo de persistencia.");
    qDebug() << "Éxito: Datos de persistencia cargados y línea restaurada.";
    return true;
}

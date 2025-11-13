#include "station.h"
#include <QThread>

Station::Station(int id, const QString& name, const QString& taskType, Buffer* input, Buffer* output, QObject* parent)
    : QThread(parent)
    , id(id)
    , name(name)
    , taskType(taskType)
    , inputBuffer(input)
    , outputBuffer(output)
    , running(false)
{
    // Constructor
}

Station::~Station() {
    // Si el hilo está corriendo, asegurar la detención segura (ProductionController debería hacerlo)
    if (isRunning()) {
        stopStation();
    }
}

void Station::run() {

    running = true;

    auto sendStatus = [&](const QString& s){
        if (lastStatus != s) {
            emit stationStatusUpdate(id, s);
            lastStatus = s;
        }
    };

    sendStatus("Activa");

    while (running) {

        Product* product = nullptr;

        // 1. Obtener producto del buffer de entrada (Bloqueante)
        if (inputBuffer) {
            sendStatus("Esperando");

            product = inputBuffer->removeProduct();

            // Si el hilo fue detenido (stopStation) mientras esperaba
            // Si el producto no es nullptr, fue obtenido just antes del 'stop',
            // pero el loop ya está saliendo, por lo que debe ser liberado
            if (!running) {
                if (product) delete product;
                break;
            }

            if (!product) {
                // Si el buffer devolviera nullptr (solo pasa con un timeout fallido en removeProduct)
                continue;
            }
        } else {
            // Este bloque no se usa en este diseño (la generación es externa)
            QThread::msleep(100);
            continue;
        }

        // 2. Procesar el producto
        sendStatus("Procesando Producto " + QString::number(product->getId()));
        processProduct(*product);

        // 3. Pasar a la siguiente etapa o finalizar
        if (outputBuffer) {
            // Estación intermedia: Pasa la propiedad del puntero al siguiente buffer
            outputBuffer->addProduct(product);
            emit productFinishedProcessing(*product, name);

        } else {
            // ÚLTIMA ESTACIÓN (STORAGE)
            // 3.1 Notifica finalización (para que el Logger actúe)
            emit productFinishedProcessing(*product, name);

            // 3.2. LIBERAR MEMORIA (único responsable de eliminar este objeto)
            qDebug() << name << ": Eliminando Producto ID:" << product->getId() << " del heap (Fin de ciclo).";
            delete product;
        }

        sendStatus("Activa");
    }

    sendStatus("Detenida");
}

void Station::stopStation() {
    running = false;

    //Despierta el hilo si está bloqueado en Buffer::removeProduct()
    if (inputBuffer) {
        //la condición del buffer sea accesible
        inputBuffer->condition.wakeAll();
    }

    // Espera a que la función run() termine
    wait();
}

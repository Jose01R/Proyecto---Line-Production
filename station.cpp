#include "station.h"

Station::Station(int id, const QString& name, const QString& taskType, Buffer* input, Buffer* output, QObject* parent)
    : QThread(parent)
    , id(id)
    , name(name)
    , taskType(taskType)
    , inputBuffer(input)
    , outputBuffer(output)
    , running(false)
{
    qDebug() << "Estación" << name << "(ID:" << id << ") creada.";
}

Station::~Station() {
    qDebug() << "Estación" << name << "(ID:" << id << ") destruida.";
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

        if (inputBuffer) {
            product = inputBuffer->removeProduct();

            if (!running) break;

            if (!product) {
                sendStatus("Esperando");   // ✅ si quieres mostrarlo
                QThread::msleep(120);
                continue;
            }
        } else {
            QThread::msleep(100);
            continue;
        }

        sendStatus("Procesando Producto " + QString::number(product->getId()));

        processProduct(*product);

        if (outputBuffer) {
            outputBuffer->addProduct(product);
        }

        emit productFinishedProcessing(*product, name);

        sendStatus("Esperando");   // ✅ si quieres mostrarlo
    }

    sendStatus("Detenida");       // ✅ si quieres mostrarlo
}




void Station::stopStation() {
    running = false;
    //wait();
}



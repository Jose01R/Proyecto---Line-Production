#include "station.h"

Station::Station(int id, const QString& name, const QString& taskType,
                 Buffer* input, Buffer* output, QObject* parent)
    : QThread(parent),
    id(id),
    name(name),
    taskType(taskType),
    inputBuffer(input),
    outputBuffer(output),
    running(false),
    lastStatus("")
{
    qDebug() << "Estación" << name << "(ID:" << id << ") creada.";
}

Station::~Station() {
    // Intentamos parar y esperar brevemente para que el hilo termine.
    stopStation();
    if (isRunning()) {
        // espera hasta 2s; si no termina, avisamos (evitar terminate salvo como último recurso)
        if (!wait(2000)) {
            qWarning() << "Station" << name << "no terminó en 2s al destruir. Podría causar crash.";
        }
    }
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

        // Si tiene buffer de entrada, intenta obtener producto
        if (inputBuffer) {

            product = inputBuffer->removeProduct();

            // Si nos pidieron parar mientras esperábamos
            if (!running) {
                break;
            }

            // Si no hay producto (timeout en buffer)
            if (!product) {
                sendStatus("Esperando");
                QThread::msleep(120);
                continue;
            }

        } else {
            // Estación sin buffer de entrada
            QThread::msleep(100);
            continue;
        }

        // ======== PROCESAR PRODUCTO ========
        sendStatus("Procesando Producto " + QString::number(product->getId()));

        processProduct(*product);  // Lógica específica (Assembler/Tester/etc)

        // Enviar producto al siguiente buffer si existe
        if (outputBuffer) {
            outputBuffer->addProduct(product);
        }

        // Notificar que este producto terminó en esta estación
        emit productFinishedProcessing(product, name);

        // La estación queda esperando siguiente producto
        sendStatus("Esperando");
    }

    // Al salir del while, estación detenida
    sendStatus("Detenida");
}

void Station::stopStation() {
    running = false;

    // Despertar el buffer de entrada (por si está bloqueada en removeProduct)
    if (inputBuffer) {
        inputBuffer->forceWake();
    }

     requestInterruption();

    // NO llamamos wait() aquí para no congelar la GUI
    // Qt se encarga de limpiar el hilo al destruir el objeto si ya terminó
}



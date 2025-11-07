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
    qDebug() << "Estación " << name << " (ID:" << id << ") creada";
}

Station::~Station() {
    qDebug() << "Estación " << name << " (ID:" << id << ") destruida";
}

void Station::run() {
    running = true;
    qDebug() << name << " (ID:" << id << ") - Hilo iniciado.";
    emit stationStatusUpdate(id, "Activa");

    while (running) {
        // 1. Tomar producto (posible bloqueo)
        Product product = inputBuffer->removeProduct();

        if (!running) break; // Reverifica la bandera después de salir del bloqueo del buffer

        // 2. Procesar
        qDebug() << name << " procesando producto ID:" << product.getId() << " (Estado:" << product.getCurrentState() << ")";
        emit stationStatusUpdate(id, "Procesando Producto " + QString::number(product.getId()));

        processProduct(product);

        // 3. Enviar
        qDebug() << name << " terminó de procesar y envió producto ID:" << product.getId() << " (Estado:" << product.getCurrentState() << ")";
        outputBuffer->addProduct(product);

        // 4. Actualizar estado
        emit productFinishedProcessing(product, name);
        emit stationStatusUpdate(id, "Esperando");
    }

    qDebug() << name << " (ID:" << id << ") - Hilo detenido.";
    emit stationStatusUpdate(id, "Detenida");
}

// IMPLEMENTACIÓN AJUSTADA para detener el hilo de forma segura
void Station::stopStation() {
    running = false;
    // Llama al nuevo método para liberar el hilo si está bloqueado esperando un producto
    if (inputBuffer) {
        inputBuffer->wakeWaiters();
    }
    //wait(); // Espera a que el método run() termine - dejar comentado
}

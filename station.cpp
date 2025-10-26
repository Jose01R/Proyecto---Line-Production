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
    qDebug() << "Estación " << name << " (ID:" << id << ") creada.";
}

Station::~Station() {
    qDebug() << "Estación " << name << " (ID:" << id << ") destruida.";
}

void Station::run() {
    running = true;
    qDebug() << name << " (ID:" << id << ") - Hilo iniciado.";
    emit stationStatusUpdate(id, "Activa"); // Notifica a la GUI que la estación está activa

    while (running) {
        // Tomar producto del buffer de entrada (bloqueante si está vacío)
        Product product = inputBuffer->removeProduct();

        if (!running) break; // Re-verifica la bandera después de salir del bloqueo del buffer

        //Procesar el producto
        qDebug() << name << " procesando producto ID:" << product.getId() << " (Estado:" << product.getCurrentState() << ")";
        emit stationStatusUpdate(id, "Procesando Producto " + QString::number(product.getId()));

        processProduct(product); // Llama a la lógica de procesamiento específica de la estación

        // 3. Enviar producto al buffer de salida
        qDebug() << name << " terminó de procesar y envió producto ID:" << product.getId() << " (Estado:" << product.getCurrentState() << ")";
        outputBuffer->addProduct(product);

        // Emite señales para actualizar la GUI
        emit productFinishedProcessing(product, name);
        emit stationStatusUpdate(id, "Esperando"); // La estación espera por el siguiente producto
    }

    qDebug() << name << " (ID:" << id << ") - Hilo detenido.";
    emit stationStatusUpdate(id, "Detenida"); // Notifica a la GUI que la estación está detenida
}

void Station::stopStation() {
    running = false;
    // Es importante "despertar" el hilo si está bloqueado esperando un producto en el inputBuffer.
    // Aunque `QWaitCondition::wakeOne()` se llama en `Buffer::addProduct()`/`removeProduct()`,
    // el `wait()` asegura que el hilo termine su ejecución actual antes de salir.
    wait(); // Espera a que el método run() del hilo termine su ejecución
}

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
    qDebug() << name << " (ID:" << id << ") - Hilo iniciado.";
    emit stationStatusUpdate(id, "Activa"); // Notifica a la GUI que la estación está activa

    while (running) {
        // Si no tiene buffer de entrada (por ejemplo, el generador), salta la lectura
        if (inputBuffer) {
            // Tomar producto del buffer de entrada (bloqueante si está vacío)
            Product* product = inputBuffer->removeProduct(); // 👈 ahora devuelve puntero

            if (!running || !product) break; // Verifica bandera y puntero

            // Procesar el producto
            qDebug() << name << " procesando producto ID:" << product->getId()
                     << "(Estado:" << product->getCurrentState() << ")";
            emit stationStatusUpdate(id, "Procesando Producto " + QString::number(product->getId()));

            processProduct(*product); // 👈 pasa referencia, sin copiar

            // Enviar producto al buffer de salida (si existe)
            if (outputBuffer) {
                qDebug() << name << " terminó de procesar y envió producto ID:" << product->getId()
                    << "(Estado:" << product->getCurrentState() << ")";
                outputBuffer->addProduct(product);
            }

            // Emite señales para actualizar la GUI
            emit productFinishedProcessing(*product, name);
            emit stationStatusUpdate(id, "Esperando"); // La estación espera por el siguiente producto
        } else {
            // Si no hay buffer de entrada (como en el generador), solo duerme un poco
            QThread::msleep(100);
        }
    }

    qDebug() << name << " (ID:" << id << ") - Hilo detenido.";
    emit stationStatusUpdate(id, "Detenida"); // Notifica a la GUI que la estación está detenida
}

void Station::stopStation() {
    running = false;

    // Despierta cualquier hilo bloqueado en wait()
    if (inputBuffer)
        inputBuffer->addProduct(nullptr); // 👈 evita bloqueo eterno

    wait(); // Espera a que el método run() del hilo termine su ejecución
}


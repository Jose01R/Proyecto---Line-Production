#ifndef PACKAGER_H
#define PACKAGER_H

#include "station.h"

class Packager : public Station {
    Q_OBJECT
public:
    Packager(int id, Buffer* input, Buffer* output, QObject* parent = nullptr)
        : Station(id, "Empaquetado", "Empaquetar", input, output, parent) {
        qDebug() << "Estación de Empaquetado (ID:" << id << ") creada.";
    }

    void processProduct(Product& product) override {
        QThread::msleep(300);
        product.advanceState();
        qDebug() << "Packager (ID:" << id << ") completó el estado:" << product.getCurrentState();
    }
};

#endif // PACKAGER_H


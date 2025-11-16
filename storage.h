#ifndef STORAGE_H
#define STORAGE_H

#include "station.h"

class Storage : public Station {
    Q_OBJECT
public:
    Storage(int id,
            Buffer* input,
            ProductionController* controller,
            QObject* parent = nullptr)
        : Station(id,
                  "Almacenamiento",
                  "Almacenar",
                  input,
                  nullptr,   // última estación no tiene buffer de salida
                  controller,
                  parent) {
        qDebug() << "Estación de Almacenamiento (ID:" << id << ") creada.";
    }

    void processProduct(Product& product) override {
        QThread::msleep(200);
        product.advanceState();
        qDebug() << "Storage (ID:" << id << ") completó el estado:" << product.getCurrentState();
    }
};

#endif // STORAGE_H


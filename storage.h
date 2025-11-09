#ifndef STORAGE_H
#define STORAGE_H

#include "station.h"

class Storage : public Station {
    Q_OBJECT
public:
    Storage(int id, Buffer* input, Buffer* output=nullptr, QObject* parent = nullptr)
        : Station(id, "Almacenamiento", "Guardar Producto", input, output, parent) {
        qDebug() << "Estación de Almacenamiento (ID:" << id << ") creada.";
    }

    void processProduct(Product& product) override {
        QThread::msleep(200);
        product.advanceState();
        qDebug() << "Storage (ID:" << id << ") completó el estado:" << product.getCurrentState();
    }
};

#endif // STORAGE_H


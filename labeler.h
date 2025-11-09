#ifndef LABELER_H
#define LABELER_H

#include "station.h"

class Labeler : public Station {
    Q_OBJECT
public:
    Labeler(int id, Buffer* input, Buffer* output, QObject* parent = nullptr)
        : Station(id, "Etiquetado", "Etiquetar", input, output, parent) {
        qDebug() << "Estación de Etiquetado (ID:" << id << ") creada.";
    }

    void processProduct(Product& product) override {
        QThread::msleep(250);
        product.advanceState();
        qDebug() << "Labeler (ID:" << id << ") completó el estado:" << product.getCurrentState();
    }
};

#endif // LABELER_H


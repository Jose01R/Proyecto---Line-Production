#ifndef TESTER_H
#define TESTER_H

#include "station.h"

/**
 * @brief Estación específica para inspeccionar la calidad de los productos.
 */

class Tester : public Station {
    Q_OBJECT // Necesario para Signals/Slots

public:
    // Constructor que llama al constructor de la clase base Station
    Tester(int id, Buffer* in, Buffer* out,
           ProductionController* controller, QObject* parent=nullptr)
        : Station(id, "Inspección de Calidad", "Inspeccionar",
                  in, out, controller, parent) {
        qDebug() << "Estación de Inspección (ID:" << id << ") creada.";
    }

    // Implementación de la lógica de procesamiento específica para la inspección
    void processProduct(Product& product) override {
        QThread::msleep(300); // Simula el tiempo que toma la inspección (0.3 segundos)
        // Aquí se podría añadir lógica para decidir si el producto pasa o falla la inspección
        // Por ahora, simplemente avanza el estado.
        product.advanceState();
        qDebug() << "Tester (ID:" << id << ") completó el estado:" << product.getCurrentState();
    }
};

#endif // TESTER_H

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "station.h"

/**
 * Estación para ensamblar piezas de productos
 */
class Assembler : public Station {
    Q_OBJECT //Signals/Slots

public:
    // Constructor que llama al constructor de clase Station
    Assembler(int id,
              Buffer* input,
              Buffer* output,
              ProductionController* controller,
              QObject* parent = nullptr)
        : Station(id,
                  "Ensamblaje",
                  "Ensamblar Pieza",
                  input,
                  output,
                  controller,   // ← ESTE parámetro es el que te faltaba
                  parent) {
        qDebug() << "Estación de Ensamblaje (ID:" << id << ") creada.";
    }

    // Implementacion de la lógica de procesamiento específica para el ensamblaje
    void processProduct(Product& product) override {
        QThread::msleep(500); // Simula el tiempo que toma el ensamblaje (0.5 segundos)
        product.advanceState(); // Avanza el estado del producto
        qDebug() << "Assembler (ID:" << id << ") completó el estado:" << product.getCurrentState();
    }
};

#endif // ASSEMBLER_H




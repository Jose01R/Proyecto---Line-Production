#ifndef BUFFER_H
#define BUFFER_H

#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QDebug>
#include "product.h"

class Buffer {
private:
    mutable QMutex mutex;
    QWaitCondition condition;
    QQueue<Product*> queue;
    int capacity;

public:
    explicit Buffer(int maxCapacity = 5);

    // Añade un producto, bloqueando si el buffer está lleno
    void addProduct(Product* product);

    // Intenta añadir un producto con timeout (ms). Devuelve true si lo logró.
    bool tryAddProduct(Product* product, int timeoutMs);

    // Saca un producto, bloqueando con timeout. Devuelve nullptr si no hay producto.
    Product* removeProduct();

    bool isEmpty() const;
    int size() const;
    int getCapacity() const { return capacity; }

    // Despierta a todos los hilos que estén esperando en este buffer
    void forceWake();
};

#endif // BUFFER_H

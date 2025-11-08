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
    QQueue<Product*> queue; // 👈 ahora punteros
    int capacity;

public:
    explicit Buffer(int maxCapacity = 5);

    void addProduct(Product* product);  // 👈 usa puntero
    Product* removeProduct();           // 👈 usa puntero

    bool isEmpty() const;
    int size() const;
    int getCapacity() const { return capacity; }
};

#endif // BUFFER_H


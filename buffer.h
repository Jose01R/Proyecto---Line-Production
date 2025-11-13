#ifndef BUFFER_H
#define BUFFER_H

#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>
#include "product.h" // Necesario para QQueue<Product*>

class Buffer : public QObject {
    Q_OBJECT

public:
    // Hacemos mutex y condition públicos para que Station::stopStation() pueda acceder a condition.wakeAll()
    //El QMutex debe ser mutable para que funcione en metodos const (como isEmpty/size).
    mutable QMutex mutex;
    QWaitCondition condition;

private:
    QQueue<Product*> queue;
    int capacity;

public:
    explicit Buffer(int maxCapacity = 5, QObject* parent = nullptr);

    void addProduct(Product* product);
    Product* removeProduct();
    bool tryAddProduct(Product* product, int timeoutMs);

    // métodos const
    bool isEmpty() const;
    int size() const;
    int getCapacity() const { return capacity; }
};

#endif // BUFFER_H

#ifndef BUFFER_H
#define BUFFER_H

#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QDebug>
#include <QJsonObject> // Añadido para JSON
#include <QJsonArray>  // Añadido para JSON
#include "product.h"

/**
 * @brief Cola protegida que sirve como enlace de comunicación sincronizado entre estaciones.
 */
class Buffer {
private:
    mutable QMutex mutex;
    QWaitCondition condition;
    QQueue<Product> queue;
    int capacity;

public:
    explicit Buffer(int maxCapacity = 5);

    // Métodos de Concurrencia
    void addProduct(const Product& product);
    Product removeProduct();

    // MÉTODO NUEVO para la detención segura de hilos bloqueados
    void wakeWaiters();

    // --- MÉTODOS DE PERSISTENCIA JSON ---
    QJsonObject toJson() const {
        QMutexLocker locker(&mutex);
        QJsonObject json;
        QJsonArray productArray;

        for (const Product& product : queue) {
            productArray.append(product.toJson());
        }

        json["capacity"] = capacity;
        json["products"] = productArray;
        return json;
    }

    void fromJson(const QJsonObject& json) {
        QMutexLocker locker(&mutex);
        queue.clear();

        if (json.contains("capacity") && json["capacity"].isDouble()) {
            capacity = json["capacity"].toInt();
        }

        if (json.contains("products") && json["products"].isArray()) {
            QJsonArray productArray = json["products"].toArray();
            for (const QJsonValue& value : productArray) {
                if (value.isObject()) {
                    Product product;
                    product.fromJson(value.toObject());
                    queue.enqueue(product);
                }
            }
        }
        condition.wakeAll();
    }
    // ------------------------------------

    // Métodos utilitarios
    bool isEmpty() const;
    int size() const;
    int getCapacity() const { return capacity; }
};

#endif // BUFFER_H

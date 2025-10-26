#ifndef BUFFER_H
#define BUFFER_H

#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QDebug>
#include "product.h" // Incluye la definición de Product

/**
 * @brief Cola protegida que sirve como enlace de comunicación sincronizado entre estaciones.
 */
class Buffer {
private:
    mutable QMutex mutex; // Para proteger el acceso a la cola (recurso compartido)
    QWaitCondition condition; // Para la sincronización Productor-Consumidor
    QQueue<Product> queue; // La cola de productos
    int capacity; // Capacidad máxima del buffer

public:
    explicit Buffer(int maxCapacity = 5); // Constructor

    // Métodos para interactuar con el buffer
    void addProduct(const Product& product); // Añade un producto (Productor)
    Product removeProduct(); // Retira un producto (Consumidor)

    // Métodos utilitarios
    bool isEmpty() const;
    int size() const;
    int getCapacity() const { return capacity; }
};

#endif // BUFFER_H

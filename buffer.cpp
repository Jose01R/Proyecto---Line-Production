#include "buffer.h"
#include <QThread>
Buffer::Buffer(int maxCapacity) : capacity(maxCapacity) {
    qDebug() << "Buffer creado con capacidad:" << capacity;
}

void Buffer::addProduct(Product* product) {
    QMutexLocker locker(&mutex);
    while (queue.size() >= capacity) {
        condition.wait(&mutex);
    }
    queue.enqueue(product);
    qDebug() << "Producto ID:" << product->getId() << " añadido. Tamaño actual:" << queue.size();
    condition.wakeOne();
}



Product* Buffer::removeProduct() {
    QMutexLocker locker(&mutex);

    while (queue.isEmpty()) {
        // Espera 100ms máx
        if (!condition.wait(&mutex, 100)) {
            return nullptr;
        }
    }

    Product* p = queue.dequeue();
    condition.wakeOne();
    return p;
}


bool Buffer::tryAddProduct(Product* product, int timeoutMs) {
    QMutexLocker locker(&mutex);

    // Espera con timeout a que haya espacio
    while (queue.size() >= capacity) {
        if (!condition.wait(&mutex, timeoutMs)) {
            return false; // no hubo espacio a tiempo
        }
    }
    queue.enqueue(product);
    qDebug() << "Producto ID:" << product->getId() << " añadido. Tamaño actual:" << queue.size();
    condition.wakeOne();
    return true;
}

bool Buffer::isEmpty() const {
    QMutexLocker locker(&mutex);
    return queue.isEmpty();
}

int Buffer::size() const {
    QMutexLocker locker(&mutex);
    return queue.size();
}


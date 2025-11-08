#include "buffer.h"

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
        condition.wait(&mutex);
    }
    Product* product = queue.dequeue();
    qDebug() << "Producto ID:" << product->getId() << " consumido. Tamaño actual:" << queue.size();
    condition.wakeOne();
    return product;
}

bool Buffer::isEmpty() const {
    QMutexLocker locker(&mutex);
    return queue.isEmpty();
}

int Buffer::size() const {
    QMutexLocker locker(&mutex);
    return queue.size();
}


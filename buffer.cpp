#include "buffer.h"
#include <QThread>

Buffer::Buffer(int maxCapacity, QObject* parent) : QObject(parent), capacity(maxCapacity) {
    qDebug() << "Buffer creado con capacidad:" << capacity;
}

void Buffer::addProduct(Product* product) {
    QMutexLocker locker(&mutex);

    while (queue.size() >= capacity) {
        qDebug() << "Buffer Lleno. Esperando para añadir Producto ID:" << product->getId();
        condition.wait(&mutex);
    }

    queue.enqueue(product);
    qDebug() << "Producto ID:" << product->getId() << " añadido. Tamaño actual:" << queue.size();
    condition.wakeOne();
}

Product* Buffer::removeProduct() {
    QMutexLocker locker(&mutex);

    while (queue.isEmpty()) {
        qDebug() << "Buffer Vacío. Estación esperando por un producto...";
        condition.wait(&mutex);
    }

    Product* p = queue.dequeue();
    qDebug() << "Producto ID:" << p->getId() << " consumido. Tamaño actual:" << queue.size();
    condition.wakeOne();
    return p;
}


bool Buffer::tryAddProduct(Product* product, int timeoutMs) {
    QMutexLocker locker(&mutex);

    while (queue.size() >= capacity) {
        if (!condition.wait(&mutex, timeoutMs)) {
            return false;
        }
    }
    queue.enqueue(product);
    qDebug() << "Producto ID:" << product->getId() << " añadido. Tamaño actual:" << queue.size();
    condition.wakeOne();
    return true;
}

//Estos métodos son const y necesitan el mutex mutable
bool Buffer::isEmpty() const {
    QMutexLocker locker(&mutex);
    return queue.isEmpty();
}

int Buffer::size() const {
    QMutexLocker locker(&mutex);
    return queue.size();
}

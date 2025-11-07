#include "buffer.h"

Buffer::Buffer(int maxCapacity) : capacity(maxCapacity) {
    qDebug() << "Buffer creado con capacidad:" << capacity;
}

void Buffer::addProduct(const Product& product) {
    QMutexLocker locker(&mutex);

    while (queue.size() >= capacity) {
        qDebug() << "Buffer Lleno (" << queue.size() << "/" << capacity << "). Esperando para añadir Producto ID:" << product.getId();
        condition.wait(&mutex);
    }

    queue.enqueue(product);
    qDebug() << "Producto ID:" << product.getId() << " añadido. Tamaño actual:" << queue.size();

    condition.wakeOne();
}

Product Buffer::removeProduct() {
    QMutexLocker locker(&mutex);

    while (queue.isEmpty()) {
        qDebug() << "Buffer Vacío. Esperando por un producto...";
        condition.wait(&mutex);
    }

    Product product = queue.dequeue();
    qDebug() << "Producto ID:" << product.getId() << " consumido. Tamaño actual:" << queue.size();

    condition.wakeOne();

    return product;
}

// IMPLEMENTACIÓN NUEVA para la detención segura
void Buffer::wakeWaiters() {
    QMutexLocker locker(&mutex);
    condition.wakeAll(); // Despierta a todos los hilos esperando en este buffer
}

bool Buffer::isEmpty() const {
    QMutexLocker locker(&mutex);
    return queue.isEmpty();
}

int Buffer::size() const {
    QMutexLocker locker(&mutex);
    return queue.size();
}

#include "buffer.h"
#include <QMutexLocker>

Buffer::Buffer(int maxCapacity)
    : capacity(maxCapacity) {
    qDebug() << "Buffer creado con capacidad:" << capacity;
}

void Buffer::addProduct(Product* product) {
    if (!product) return; // por seguridad, no encolamos nullptr

    QMutexLocker locker(&mutex);

    // Si está lleno, esperar hasta que haya espacio
    while (queue.size() >= capacity) {
        condition.wait(&mutex);
    }

    queue.enqueue(product);
    qDebug() << "Producto ID:" << product->getId()
             << "añadido. Tamaño actual:" << queue.size();

    condition.wakeOne(); // despierta a un consumidor
}

bool Buffer::tryAddProduct(Product* product, int timeoutMs) {
    if (!product) return false;

    QMutexLocker locker(&mutex);

    while (queue.size() >= capacity) {
        // Espera tiempo limitado
        if (!condition.wait(&mutex, timeoutMs)) {
            // No hubo espacio en el tiempo dado
            return false;
        }
    }

    queue.enqueue(product);
    qDebug() << "Producto ID:" << product->getId()
             << "añadido (try). Tamaño actual:" << queue.size();

    condition.wakeOne();
    return true;
}

Product* Buffer::removeProduct() {
    QMutexLocker locker(&mutex);

    // Espera mientras esté vacío, pero con timeout para poder salir
    while (queue.isEmpty()) {
        if (!condition.wait(&mutex, 100)) {
            // Timeout: no hay producto
            return nullptr;
        }
    }

    Product* p = queue.dequeue();
    condition.wakeOne(); // despierta a un productor que esté esperando
    return p;
}

bool Buffer::isEmpty() const {
    QMutexLocker locker(&mutex);
    return queue.isEmpty();
}

int Buffer::size() const {
    QMutexLocker locker(&mutex);
    return queue.size();
}

void Buffer::forceWake() {
    QMutexLocker locker(&mutex);
    // Despierta a TODOS los que estén esperando en este buffer
    condition.wakeAll();
}

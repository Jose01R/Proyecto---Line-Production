#include "buffer.h"

Buffer::Buffer(int maxCapacity) : capacity(maxCapacity) {
    qDebug() << "Buffer creado con capacidad:" << capacity;
}

void Buffer::addProduct(const Product& product) {
    QMutexLocker locker(&mutex); // Bloquea el mutex al entrar, lo libera al salir del scope

    while (queue.size() >= capacity) {
        qDebug() << "Buffer Lleno (" << queue.size() << "/" << capacity << "). Esperando para añadir Producto ID:" << product.getId();
        condition.wait(&mutex); // Libera el mutex y espera; lo readquiere al despertar
    }

    queue.enqueue(product);
    qDebug() << "Producto ID:" << product.getId() << " añadido. Tamaño actual:" << queue.size();

    condition.wakeOne(); // Notifica a un hilo en espera (consumidor)
}

Product Buffer::removeProduct() {
    QMutexLocker locker(&mutex); // Bloquea el mutex al entrar, lo libera al salir del scope

    while (queue.isEmpty()) {
        qDebug() << "Buffer Vacío. Esperando por un producto...";
        condition.wait(&mutex); // Libera el mutex y espera; lo readquiere al despertar
    }

    Product product = queue.dequeue();
    qDebug() << "Producto ID:" << product.getId() << " consumido. Tamaño actual:" << queue.size();

    condition.wakeOne(); // Notifica a un hilo en espera (productor)

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


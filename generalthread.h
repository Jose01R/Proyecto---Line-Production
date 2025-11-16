#ifndef GENERALTHREAD_H
#define GENERALTHREAD_H

#include <QThread>
#include <QString>
#include <QDebug>
#include <QMutex>

/**
 * Clase base para los hilos de mantenimiento.
 * Proporciona funcionalidad de parada segura y emisión de mensajes.
 */
class GeneralThread : public QThread {
    Q_OBJECT // Necesario para usar señales y slots

protected:
    // Propiedades comunes
    QString threadName;
    bool isRunning;
    QMutex mutex; // Para asegurar acceso a propiedades compartidas (como isRunning)

public:
    // Constructor
    GeneralThread(const QString& name, QObject* parent = nullptr)
        : QThread(parent), threadName(name), isRunning(true) {}

    // Destructor virtual
    virtual ~GeneralThread() override {
        // Asegura que el hilo se detenga antes de la destrucción
        stopSafely();
        wait();
    }

    // Método para detener el hilo de forma segura
    void stopSafely() {
        QMutexLocker locker(&mutex);
        isRunning = false;
    }

    // Getter para el nombre
    QString getName() const { return threadName; }

    // El método run() será implementado por las clases derivadas.
    // virtual void run() override = 0; // Se define como 0 si es puramente abstracta,
    // pero la dejamos sin implementar para evitar
    // problemas si la clase base necesita alguna lógica.

signals:
    // Señal para lanzar mensajes a la interfaz gráfica (bitácora, advertencias, etc.)
    void threadMessage(const QString& name, const QString& action, const QString& message);

protected:
    // Método auxiliar para emitir mensajes con el formato requerido
    void emitMessage(const QString& action, const QString& message = "") {
        emit threadMessage(threadName, action, message);
    }
};

#endif // GENERALTHREAD_H

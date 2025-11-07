#ifndef STATION_H
#define STATION_H

#include <QThread>
#include <QString>
#include <QDebug>
#include <QJsonObject> // Añadido para JSON
#include "buffer.h"
#include "product.h"

/**
 * @brief Clase base que representa una estación de trabajo (hilo) en la línea de producción.
 */
class Station : public QThread {
    Q_OBJECT

protected:
    int id;
    QString name;
    QString taskType;
    Buffer* inputBuffer;
    Buffer* outputBuffer;
    volatile bool running;

    // Constructor base por defecto (útil para la deserialización)
    Station(QObject* parent = nullptr) : QThread(parent), id(0), name(""), taskType(""), inputBuffer(nullptr), outputBuffer(nullptr), running(false) {}


public:
    // Constructor existente
    Station(int id, const QString& name, const QString& taskType, Buffer* input, Buffer* output, QObject* parent = nullptr);
    ~Station() override;

    // --- MÉTODOS DE PERSISTENCIA JSON ---
    virtual QJsonObject toJson() const {
        QJsonObject json;
        json["id"] = id;
        json["name"] = name;
        json["taskType"] = taskType;
        // La clase real (Assembler, Tester) es esencial para la recreación
        json["className"] = QString(metaObject()->className());
        return json;
    }

    virtual void fromJson(const QJsonObject& json) {
        if (json.contains("id") && json["id"].isDouble()) {
            id = json["id"].toInt();
        }
        if (json.contains("name") && json["name"].isString()) {
            name = json["name"].toString();
        }
        if (json.contains("taskType") && json["taskType"].isString()) {
            taskType = json["taskType"].toString();
        }
        // Los punteros de buffer se reasignan en ProductionController::loadDataFromJson()
    }
    // ------------------------------------

    virtual void processProduct(Product& product) = 0;

    void run() override;
    void stopStation(); // Asegura la detención segura usando Buffer::wakeWaiters()

    QString getName() const { return name; }
    int getId() const { return id; }

signals:
    void stationStatusUpdate(int stationId, const QString& status);
    void productFinishedProcessing(const Product& product, const QString& stationName);
};

#endif // STATION_H

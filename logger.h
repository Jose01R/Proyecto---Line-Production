#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDateTime>
#include "product.h" // Usaremos Product::toJson()

// Estructura de datos para el registro en memoria
struct DailyLogEntry {
    QDateTime completionTime; // Fecha y hora en que terminó
    int productId;
    QString productType;
    QString finalStatus;
    // NUEVO: Podríamos guardar el tiempo de creación del producto aquí también
    QDateTime creationTime;
};

class Logger : public QObject {
    Q_OBJECT

private:
    QString logFileName = "production_log.json";
    QList<DailyLogEntry> dailyLog;

public:
    explicit Logger(QObject* parent = nullptr);

    // Métodos de Persistencia
    void saveLogToJson() const;
    void loadLogFromJson();

    // Método para ser llamado por ProductionController/Storage al terminar un producto
    void recordCompletion(const Product& product);

    QList<DailyLogEntry> getDailyLog() const { return dailyLog; }

signals:
    // Señal para notificar a la GUI que un producto fue registrado
    void newLogEntry(const QString& message);
};

#endif // LOGGER_H

#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDateTime>
#include <QMap>
#include <QDate>

#include "product.h"

// Registro por producto terminado
struct DailyLogEntry {
    QDateTime completionTime;
    int productId;
    QString productType;
    QString finalStatus;
    QDateTime creationTime;
};

class Logger : public QObject {
    Q_OBJECT

private:
    QString logFileName = "production_log.json";
    QList<DailyLogEntry> dailyLog;

    // Estadísticas diarias en memoria:
    // dailyTotalCount["2025-11-15"] = 8
    QMap<QString, int> dailyTotalCount;

    // dailyTypeCount["2025-11-15"]["Lavadora"] = 5
    QMap<QString, QMap<QString,int>> dailyTypeCount;

    // Fecha actual en memoria
    QString todayString() const { return QDate::currentDate().toString(Qt::ISODate); }

public:
    explicit Logger(QObject* parent = nullptr);

    // Persistencia
    void saveLogToJson() const;
    void loadLogFromJson();

    // Llamar cuando un producto termina (estado Almacenado)
    void recordCompletion(const Product& product);

    QList<DailyLogEntry> getDailyLog() const { return dailyLog; }

    // Accesores para UI o reportes
    int getTotalProducedForDate(const QString& date) const;
    QMap<QString,int> getTypeCountsForDate(const QString& date) const;

signals:
    void newLogEntry(const QString& message);
};

#endif // LOGGER_H

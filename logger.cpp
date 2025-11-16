#include "logger.h"
#include <QJsonValue>

Logger::Logger(QObject* parent) : QObject(parent) {
    loadLogFromJson();
    qDebug() << "Logger inicializado. Registros cargados:" << dailyLog.size();
}

/**
 * Registrar producto finalizado. Solo registra si estado == "Almacenado"
 */
void Logger::recordCompletion(const Product& product) {
    if (product.getCurrentState() != "Almacenado") {
        return;
    }

    DailyLogEntry entry;
    entry.completionTime = QDateTime::currentDateTime();
    entry.productId = product.getId();
    entry.productType = product.getType();
    entry.finalStatus = product.getCurrentState();
    entry.creationTime = product.getCreationTime();

    dailyLog.append(entry);

    // Actualizar estadísticas diarias en memoria
    QString dateKey = todayString();
    dailyTotalCount[dateKey] = dailyTotalCount.value(dateKey, 0) + 1;
    dailyTypeCount[dateKey][entry.productType] = dailyTypeCount[dateKey].value(entry.productType, 0) + 1;

    // Mensaje para GUI
    QString message = QString("%1 | ID:%2 | Tipo:%3 | Creación:%4")
                          .arg(entry.completionTime.toString("dd/MM hh:mm:ss"))
                          .arg(entry.productId)
                          .arg(entry.productType)
                          .arg(entry.creationTime.toString("hh:mm:ss"));

    emit newLogEntry(message);

    // Guardar inmediatamente
    saveLogToJson();
}

/**
 * Guarda dailyLog y dailyStats en un objeto JSON estructurado:
 * {
 *   "logs": [ ... ],
 *   "dailyStats": {
 *       "2025-11-15": { "totalProduced": 8, "types": {"Lavadora":5, ...} },
 *       ...
 *   }
 * }
 */
void Logger::saveLogToJson() const {
    QJsonArray logArray;
    for (const auto& entry : dailyLog) {
        QJsonObject json;
        json["completionTime"] = entry.completionTime.toString(Qt::ISODate);
        json["productId"] = entry.productId;
        json["productType"] = entry.productType;
        json["finalStatus"] = entry.finalStatus;
        json["creationTime"] = entry.creationTime.toString(Qt::ISODate);
        logArray.append(json);
    }

    QJsonObject root;
    root["logs"] = logArray;

    // dailyStats
    QJsonObject statsObj;
    for (auto it = dailyTotalCount.constBegin(); it != dailyTotalCount.constEnd(); ++it) {
        QString dateKey = it.key();
        int total = it.value();

        QJsonObject dayObj;
        dayObj["totalProduced"] = total;

        QJsonObject typesObj;
        QMap<QString,int> typeMap = dailyTypeCount.value(dateKey);
        for (auto jt = typeMap.constBegin(); jt != typeMap.constEnd(); ++jt) {
            typesObj[jt.key()] = jt.value();
        }
        dayObj["types"] = typesObj;

        statsObj[dateKey] = dayObj;
    }
    root["dailyStats"] = statsObj;

    QJsonDocument doc(root);
    QFile file(logFileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        qDebug() << "Log guardado en" << logFileName;
    } else {
        qWarning() << "Error al abrir el archivo de log para escritura.";
    }
}

/**
 * Carga logs y estadísticas desde JSON.
 * Si existe información para la fecha actual, la carga en memoria para continuar acumulando.
 */
void Logger::loadLogFromJson() {
    QFile file(logFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "No se encontró el archivo de log JSON o no se pudo abrir (se creará uno nuevo al guardar).";
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "Error al parsear el archivo JSON o formato inesperado.";
        return;
    }

    QJsonObject root = doc.object();

    // Cargar logs
    dailyLog.clear();
    if (root.contains("logs") && root["logs"].isArray()) {
        QJsonArray logArray = root["logs"].toArray();
        for (const QJsonValue& v : logArray) {
            if (!v.isObject()) continue;
            QJsonObject json = v.toObject();
            DailyLogEntry entry;
            entry.completionTime = QDateTime::fromString(json["completionTime"].toString(), Qt::ISODate);
            entry.productId = json["productId"].toInt();
            entry.productType = json["productType"].toString();
            entry.finalStatus = json["finalStatus"].toString();
            entry.creationTime = QDateTime::fromString(json["creationTime"].toString(), Qt::ISODate);
            dailyLog.append(entry);
        }
    }

    // Cargar dailyStats
    dailyTotalCount.clear();
    dailyTypeCount.clear();
    if (root.contains("dailyStats") && root["dailyStats"].isObject()) {
        QJsonObject statsObj = root["dailyStats"].toObject();
        for (auto it = statsObj.constBegin(); it != statsObj.constEnd(); ++it) {
            QString dateKey = it.key();
            QJsonObject dayObj = it.value().toObject();
            int total = dayObj.value("totalProduced").toInt(0);
            dailyTotalCount[dateKey] = total;

            QMap<QString,int> typeMap;
            if (dayObj.contains("types") && dayObj["types"].isObject()) {
                QJsonObject typesObj = dayObj["types"].toObject();
                for (auto jt = typesObj.constBegin(); jt != typesObj.constEnd(); ++jt) {
                    typeMap[jt.key()] = jt.value().toInt(0);
                }
            }
            dailyTypeCount[dateKey] = typeMap;
        }
    }
}

/* Accessors para UI */
int Logger::getTotalProducedForDate(const QString& date) const {
    return dailyTotalCount.value(date, 0);
}

QMap<QString,int> Logger::getTypeCountsForDate(const QString& date) const {
    return dailyTypeCount.value(date, QMap<QString,int>());
}

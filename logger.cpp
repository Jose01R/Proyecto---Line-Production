#include "logger.h"

Logger::Logger(QObject* parent) : QObject(parent) {
    // Al iniciar, intentamos cargar los registros guardados
    loadLogFromJson();
    qDebug() << "Logger inicializado. Registros cargados:" << dailyLog.size();
}

/**
 * @brief Registra un producto que ha llegado al estado final (Almacenado).
 * @param product El producto terminado.
 */
void Logger::recordCompletion(const Product& product) {
    if (product.getCurrentState() != "Almacenado") {
        return;
    }

    DailyLogEntry entry;
    entry.completionTime = QDateTime::currentDateTime(); //Hora de FINALIZACIÓN
    entry.productId = product.getId();
    entry.productType = product.getType();
    entry.finalStatus = product.getCurrentState();
    entry.creationTime = product.getCreationTime(); //Hora de CREACIÓN (desde Product)

    dailyLog.append(entry);

    // Mensaje para interfaz gráfica
    QString message = QString("%1 | ID:%2 | Tipo:%3 | Creación:%4")
                          .arg(entry.completionTime.toString("dd/MM hh:mm:ss"))
                          .arg(entry.productId)
                          .arg(entry.productType)
                          .arg(entry.creationTime.toString("hh:mm:ss")); // Mostramos hora de creación

    emit newLogEntry(message);

    // Guardar inmediatamente para persistencia
    saveLogToJson();
}

/**
 * Guarda la lista de registros (dailyLog) en un archivo JSON
 */
void Logger::saveLogToJson() const {
    QJsonArray logArray;

    for (const auto& entry : dailyLog) {
        QJsonObject json;
        // La fecha de finalización
        json["completionTime"] = entry.completionTime.toString(Qt::ISODate);
        json["productId"] = entry.productId;
        json["productType"] = entry.productType;
        json["finalStatus"] = entry.finalStatus;
        // La fecha y hora de creación del producto
        json["creationTime"] = entry.creationTime.toString(Qt::ISODate);
        logArray.append(json);
    }

    QJsonDocument doc(logArray);
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
 * Carga los registros desde el archivo JSON al iniciar la aplicación.
 */
void Logger::loadLogFromJson() {
    QFile file(logFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "No se encontró el archivo de log JSON o no se pudo abrir";
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isArray()) {
        qWarning() << "Error al parsear el archivo JSON";
        return;
    }

    dailyLog.clear();
    QJsonArray logArray = doc.array();

    for (const QJsonValue& value : logArray) {
        QJsonObject json = value.toObject();
        DailyLogEntry entry;

        // Carga la fecha y hora de finalización
        entry.completionTime = QDateTime::fromString(json["completionTime"].toString(), Qt::ISODate);
        entry.productId = json["productId"].toInt();
        entry.productType = json["productType"].toString();
        entry.finalStatus = json["finalStatus"].toString();

        // Carga la fecha y hora de creación
        entry.creationTime = QDateTime::fromString(json["creationTime"].toString(), Qt::ISODate);

        dailyLog.append(entry);
    }
}

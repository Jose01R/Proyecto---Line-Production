#include "product.h"
#include <QJsonDocument>

// Constructor principal: Inicializa tiempo de creación
Product::Product(int id, const QString& type)
    : id(id), type(type), currentState("Inicial"), creationTime(QDateTime::currentDateTime()) {}

// Constructor default (para señales y deserialización)
Product::Product()
    : id(-1), type("Undefined"), currentState("Inicial"), creationTime(QDateTime()) {}

/**
 * Avanza el producto al siguiente estado
 * Se ajusta a la línea de 5 estaciones
 */
void Product::advanceState() {
    if (currentState == "Inicial") {
        currentState = "Ensamblado";
    } else if (currentState == "Ensamblado") {
        currentState = "Inspeccionado";
    } else if (currentState == "Inspeccionado") {
        currentState = "Etiquetado";
    } else if (currentState == "Etiquetado") {
        currentState = "Empaquetado";
    } else if (currentState == "Empaquetado") {
        currentState = "Almacenado"; // Estado final
    }
}

/**
 * Devuelve una cadena de información básica
 */
QString Product::showInfo() const {
    return QString("ID: %1, Tipo: %2, Estado: %3, Creado: %4")
    .arg(id).arg(type).arg(currentState).arg(creationTime.toString("hh:mm:ss"));
}

// ====================================================================
// LÓGICA DE PERSISTENCIA (JSON)
// ====================================================================

/**
 * Convierte el objeto Product a un QJsonObject para guardar en JSON.
 */
QJsonObject Product::toJson() const {
    QJsonObject json;
    json["id"] = id;
    json["type"] = type;
    json["currentState"] = currentState;

    // Serialización de la FECHA Y HORA DE CREACIÓN
    json["creationTime"] = creationTime.toString(Qt::ISODate);

    return json;
}

/**
 * Crea un objeto Product a partir de un QJsonObject (para cargar desde JSON).
 */
Product Product::fromJson(const QJsonObject& json) {
    int id = json["id"].toInt();
    QString type = json["type"].toString();
    QString state = json["currentState"].toString();

    // Deserialización de la FECHA Y HORA DE CREACIÓN
    QDateTime time = QDateTime::fromString(json["creationTime"].toString(), Qt::ISODate);

    Product product(id, type);
    product.currentState = state;
    product.creationTime = time;
    return product;
}

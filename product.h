#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>
#include <QDebug>
#include <QJsonObject> // Añadido para serialización

/**
 * @brief Representa un producto (electrodoméstico) que pasa por la línea de producción.
 */
class Product {
private:
    int id; // Identificador único del producto
    QString type; // Tipo de electrodoméstico (ej: "Refrigerador")
    QString currentState; // Estado actual del montaje

public:
    // CONSTRUCTOR para inicialización
    Product(int id, const QString& type) : id(id), type(type), currentState("Inicial") {}

    // CONSTRUCTOR por defecto (necesario para la deserialización)
    Product() : id(0), type("Unknown"), currentState("Inicial") {}

    // MÉTODO NUEVO para SERIALIZACIÓN (Guardar)
    QJsonObject toJson() const {
        QJsonObject json;
        json["id"] = id;
        json["type"] = type;
        json["currentState"] = currentState;
        return json;
    }

    // MÉTODO NUEVO para DESERIALIZACIÓN (Cargar)
    void fromJson(const QJsonObject& json) {
        if (json.contains("id") && json["id"].isDouble()) {
            id = json["id"].toInt();
        }
        if (json.contains("type") && json["type"].isString()) {
            type = json["type"].toString();
        }
        if (json.contains("currentState") && json["currentState"].isString()) {
            currentState = json["currentState"].toString();
        }
    }

    // Lógica y Getters (se mantienen igual)
    void advanceState() {
        if (currentState == "Inicial") {
            currentState = "Ensamblado";
        } else if (currentState == "Ensamblado") {
            currentState = "Inspeccionado";
        } else if (currentState == "Inspeccionado") {
            currentState = "Empaquetado";
        } else if (currentState == "Empaquetado") {
            currentState = "Terminado";
        }
        qDebug() << "Producto ID:" << id << " avanzó a estado:" << currentState;
    }

    QString showInfo() const {
        return QString("ID: %1, Tipo: %2, Estado: %3").arg(id).arg(type).arg(currentState);
    }
    int getId() const { return id; }
    QString getType() const { return type; }
    QString getCurrentState() const { return currentState; }
};

#endif // PRODUCT_H

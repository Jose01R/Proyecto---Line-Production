#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>
#include <QDebug> // Para propósitos de depuración

/**
 * @brief Representa un producto (electrodoméstico) que pasa por la línea de producción.
 */
class Product {
private:
    int id; // Identificador único del producto
    QString type; // Tipo de electrodoméstico (ej: "Refrigerador")
    QString currentState; // Estado actual del montaje (ej: "Inicial", "Ensamblado", "Inspeccionado")

public:
    //CONSTRUCTOR
    Product(int id, const QString& type) : id(id), type(type), currentState("Inicial") {}

    // Lógica para avanzar el estado del producto
    void advanceState() {
        if (currentState == "Inicial") {
            currentState = "Ensamblado";
        } else if (currentState == "Ensamblado") {
            currentState = "Inspeccionado";
        } else if (currentState == "Inspeccionado") {
            currentState = "Empaquetado";
        } else if (currentState == "Empaquetado") {
            currentState = "Terminado"; // Estado final
        }
        qDebug() << "Producto ID:" << id << " avanzó a estado:" << currentState;
    }

    // Información del producto para depuración o GUI
    QString showInfo() const {
        return QString("ID: %1, Tipo: %2, Estado: %3").arg(id).arg(type).arg(currentState);
    }

    // Getters
    int getId() const { return id; }
    QString getType() const { return type; }
    QString getCurrentState() const { return currentState; }
};

#endif // PRODUCT_H

#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>
#include <QDebug>
#include <QDateTime>   //Para registrar la fecha y hora de creación
#include <QJsonObject> //Para serialización JSON

class Product {
private:
    int id;
    QString type;
    QString currentState;
    QDateTime creationTime; //Para el registro de tiempo

public:
    // CONSTRUCTORES: Inicializan el tiempo al momento de la creación
    Product(int id, const QString& type);
    Product(); // Constructor por defecto

    // LÓGICA DE ESTADO (para 5 estaciones: Ensamblado, Inspeccionado, Etiquetado, Empaquetado, Almacenado)
    void advanceState();
    QString showInfo() const;

    // MÉTODOS PARA JSON
    QJsonObject toJson() const;
    static Product fromJson(const QJsonObject& json);

    // GETTERS
    int getId() const { return id; }
    QString getType() const { return type; }
    QString getCurrentState() const { return currentState; }
    QDateTime getCreationTime() const { return creationTime; }
};

#endif // PRODUCT_H

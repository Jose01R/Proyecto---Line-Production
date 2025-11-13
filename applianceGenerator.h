#ifndef APPLIANCEGENERATOR_H
#define APPLIANCEGENERATOR_H

#include <QList>
#include <QString>
#include <QRandomGenerator> // Para seleccionar tipos aleatorios
#include <QDebug>
#include "product.h" //Necesita saber qué es Product

class ApplianceGenerator {
private:
    QList<QString> applianceTypes; // Lista de posibles tipos de productos
    int nextProductId;             // Contador para asignar IDs únicos

public:
    ApplianceGenerator();

    /**
     * Crea y devuelve un nuevo objeto Product con un ID único y tipo aleatorio.
     * * Este método usa 'new', por lo que el puntero devuelto
     * debe ser eliminado con 'delete' por la estación final (Storage)
     * para evitar fugas de memoria
     * * @return Puntero al nuevo objeto Product
     */
    Product* generateNewProduct();

    int getLastProductId() const { return nextProductId - 1; }
};

#endif // APPLIANCEGENERATOR_H

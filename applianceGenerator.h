#ifndef APPLIANCEGENERATOR_H
#define APPLIANCEGENERATOR_H

#include <QList>
#include <QString>
#include <QRandomGenerator>
#include <QDebug>
#include <QMap>
#include <QDate>

#include "product.h"

class ApplianceGenerator {
private:
    QList<QString> applianceTypes;      // Lista de tipos posibles
    QMap<QString,int> nextIdByType;     // nextIdByType["Lavadora"] = siguiente id (ej: 4)
    QDate currentDate;                  // Fecha en la que se están asignando IDs (día actual)

    // Internamente podemos usar esto para debug
    void ensureDateIsCurrent();

public:
    ApplianceGenerator();

    /**
     * Crea y devuelve un nuevo objeto Product con un ID único y tipo aleatorio.
        Este método usa 'new', por lo que el puntero devuelto
        debe ser eliminado con 'delete' por la estación final (Storage)
        para evitar fugas de memoria
     @return Puntero al nuevo objeto Product
    */

    // Crear nuevo Product; ahora los IDs son por tipo y se reinician al cambiar el día
    Product* generateNewProduct();

    // Para consultar el último ID asignado globalmente (no usado pero útil)
    int getLastProductIdForType(const QString& type) const;

    // Opcional: permitir conocer la lista de tipos
    QList<QString> getApplianceTypes() const { return applianceTypes; }
};

#endif // APPLIANCEGENERATOR_H

#include "applianceGenerator.h"

/**
 * @brief Crea un nuevo producto con un ID único y un tipo aleatorio.
 */

ApplianceGenerator::ApplianceGenerator()
    : currentDate(QDate::currentDate())
{
    applianceTypes << "Refrigerador" << "Lavadora" << "Microondas" << "Televisor";
    // Inicializar nextIdByType en 1 para cada tipo
    for (const QString& t : applianceTypes) {
        nextIdByType[t] = 1;
    }
}

/**
 * Verifica si la fecha del sistema cambió respecto a currentDate.
 * Si cambió, reinicia los contadores por tipo (IDs vuelven a 1).
 */
void ApplianceGenerator::ensureDateIsCurrent() {
    QDate now = QDate::currentDate();
    if (now != currentDate) {
        qDebug() << "Nuevo día detectado. Reiniciando IDs por tipo. (" << currentDate << "->" << now << ")";
        currentDate = now;
        nextIdByType.clear();
        for (const QString& t : applianceTypes) {
            nextIdByType[t] = 1;
        }
    }
}

Product* ApplianceGenerator::generateNewProduct() {
    if (applianceTypes.isEmpty()) {
        qWarning() << "Error: No hay tipos de electrodomésticos definidos.";
        return nullptr;
    }

    // Si cambió el día, reiniciamos los ID por tipo
    ensureDateIsCurrent();

    // Seleccionar tipo aleatorio
    int randomIndex = QRandomGenerator::global()->bounded(applianceTypes.size());
    QString type = applianceTypes.at(randomIndex);

    // Obtener ID por tipo
    int idForType = nextIdByType.value(type, 1);

    // Crear producto con ese ID (ID empieza en 1 para cada tipo cada día)
    Product* newProduct = new Product(idForType, type);

    // Incrementar el contador para el tipo
    nextIdByType[type] = idForType + 1;

    return newProduct;
}

int ApplianceGenerator::getLastProductIdForType(const QString& type) const {
    if (nextIdByType.contains(type)) {
        return nextIdByType[type] - 1;
    }
    return 0;
}

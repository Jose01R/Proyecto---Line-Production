#include "applianceGenerator.h".h"

ApplianceGenerator::ApplianceGenerator() : nextProductId(1) {
    // Definimos los tipos de electrodomésticos para simulación
    applianceTypes << "Refrigerador" << "Lavadora" << "Microondas" << "Televisor";
    // SE PUEDEN ANNADIR MAS TIPOS
}

/**
 * @brief Crea un nuevo producto con un ID único y un tipo aleatorio.
 */
Product* ApplianceGenerator::generateNewProduct() {
    if (applianceTypes.isEmpty()) {
        qWarning() << "Error: No hay tipos de electrodomésticos definidos.";
        return nullptr;
    }

    // Seleccionar un tipo aleatorio
    // Genera un índice entre 0 y applianceTypes.size() - 1
    int randomIndex = QRandomGenerator::global()->bounded(applianceTypes.size());
    QString type = applianceTypes.at(randomIndex);

    // Crear el nuevo objeto Product en el heap (con 'new')
    Product* newProduct = new Product(nextProductId, type);

    // Incrementar el contador para el próximo producto
    nextProductId++;

    return newProduct;
}

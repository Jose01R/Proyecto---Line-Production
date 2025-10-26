#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QLabel> // Si usas etiquetas de estado
// Nota: ProductionController.h ya incluye las demás clases (Product, Station, Buffer)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. INSTANCIAR Y CONFIGURAR EL CONTROLADOR
    // Pasa 'this' para que MainWindow sea el padre y maneje la memoria
    controller = new ProductionController(this);

    // Configura la línea con el número de estaciones que desees probar (ej. 4 estaciones)
    controller->setupProductionLine(4);

    qDebug() << "--- SIMULACIÓN LISTA PARA INICIAR ---";

    // 2. CONEXIONES CLAVE PARA EL CONTROL Y EL TESTEO POR CONSOLA

    // A) CONEXIÓN DE BOTONES: Conecta los botones de tu GUI directamente a los slots del Controller.
    // **Ajusta los nombres 'pushButton_Start' y 'pushButton_Stop' a los nombres de tus botones.**

    // Conectar botón de INICIO al slot del controlador
    connect(ui->pushButton_Start, &QPushButton::clicked,
            controller, &ProductionController::startProduction);

    // Conectar botón de DETENER al slot del controlador
    connect(ui->pushButton_Stop, &QPushButton::clicked,
            controller, &ProductionController::stopProduction);

    // B) CONEXIÓN DE LOGGING (Prueba de Consola): Muestra el flujo en la consola
    // Conectamos la señal de avance de producto para ver el log de concurrencia:
    connect(controller, &ProductionController::productStateChanged,
            [](const Product& product, const QString& stationName) {
                // Este mensaje aparecerá en la consola "Application Output"
                qDebug() << "LOG CONCURRENTE:"
                         << "ID:" << product.getId()
                         << "| Estación:" << stationName
                         << "| NUEVO ESTADO:" << product.getCurrentState();
            });

    // Opcional: Conectar la señal de estado de la línea a una etiqueta (si tienes una QLabel)
    // connect(controller, &ProductionController::productionLineStatus, ui->statusLabel, &QLabel::setText);
}

MainWindow::~MainWindow()
{
    // Asegura una detención segura de los hilos antes de que la aplicación termine
    controller->stopProduction();
    delete ui;
}

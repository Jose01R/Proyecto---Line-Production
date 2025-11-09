#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QLabel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. INSTANCIAR EL CONTROLADOR
    controller = new ProductionController(this);

    // 2. LÓGICA DE PERSISTENCIA: INTENTAR CARGAR DATOS
    bool loaded = controller->loadDataFromJson();

    // 3. CONFIGURAR O INICIAR
    if (!loaded) {
        qDebug() << "Usando configuración de línea por defecto...";
        controller->setupProductionLine(4); // Configura la línea por defecto
    } else {
        qDebug() << "Línea de producción restaurada exitosamente.";
    }

    qDebug() << "--- SIMULACIÓN LISTA PARA INICIAR ---";

    // 4. CONEXIONES CLAVE
    connect(ui->pushButton_Start, &QPushButton::clicked,
            controller, &ProductionController::startProduction);

    connect(ui->pushButton_Stop, &QPushButton::clicked,
            controller, &ProductionController::stopProduction);

    // Conexión de logging (se mantiene igual)
    connect(controller, &ProductionController::productStateChanged,
            [](const Product& product, const QString& stationName) {
                qDebug() << "LOG CONCURRENTE:"
                         << "ID:" << product.getId()
                         << "| Estación:" << stationName
                         << "| NUEVO ESTADO:" << product.getCurrentState();
            });
}

MainWindow::~MainWindow()
{
    // closeEvent() maneja la detención y el guardado, aquí solo limpiamos UI.
    delete ui;
}

/**
 * @brief Maneja el evento de cierre de la ventana: detiene hilos y guarda el estado.
 */
void MainWindow::closeEvent(QCloseEvent *event) {
    qDebug() << "Evento de Cierre detectado. Guardando estado y deteniendo hilos...";

    // Detener la producción de forma segura
    controller->stopProduction();

    // Guardar el estado
    if (controller->saveDataToJson()) {
        QMessageBox::information(this, "Guardado Exitoso",
                                 "El estado de la línea de producción ha sido guardado.",
                                 QMessageBox::Ok);
        event->accept(); // Permite que la ventana se cierre
    } else {
        QMessageBox::critical(this, "Error de Guardado",
                              "No se pudo guardar el estado de la línea de producción.",
                              QMessageBox::Ok);
        event->accept(); // Aún permitimos el cierre para evitar bloqueo
    }
}

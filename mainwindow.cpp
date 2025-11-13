#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include "logger.h" //

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , controller(new ProductionController(this))
    , metricsTimer(new QTimer(this))
    , productsProcessed(0)
{
    ui->setupUi(this);

    ui->label_Status->setText("Sistema detenido");
    ui->textEdit_Log->setReadOnly(true);
    ui->tableWidget_Threads->setRowCount(5);

    // Crear línea de producción con 5 estaciones mínimo
    controller->setupProductionLine(5);

    setupConnections();

    connect(metricsTimer, &QTimer::timeout, this, &MainWindow::updateMetrics);

    // Al iniciar, cargamos los logs existentes para ver el historial
    ui->textEdit_Log->append(">>> Cargando log de persistencia JSON... <<<");
    QList<DailyLogEntry> logs = controller->getLogger()->getDailyLog();
    for (const auto& entry : logs) {
        QString msg = QString("[JSON HISTÓRICO %1] Producto ID:%2 | Tipo:%3 | Creado:%4 | Finalizado:%5")
                          .arg(entry.completionTime.toString("dd/MM"))
                          .arg(entry.productId)
                          .arg(entry.productType)
                          .arg(entry.creationTime.toString("hh:mm:ss"))
                          .arg(entry.completionTime.toString("hh:mm:ss"));
        ui->textEdit_Log->append(msg);
    }
    ui->textEdit_Log->append(QString(">>> %1 registros cargados. <<<").arg(logs.size()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupConnections() {

    connect(controller, &ProductionController::updateStationStatus,
            this, &MainWindow::onStationStatusUpdate);

    connect(controller, &ProductionController::productStateChanged,
            this, &MainWindow::onProductFinishedProcessing);

    connect(controller, &ProductionController::productionLineStatus,
            this, &MainWindow::onProductionLineStatus);

    //Conecta la señal de registro del Logger al slot de la GUI
    // controller->getLogger() devuelve el puntero al objeto Logger
    connect(controller->getLogger(), &Logger::newLogEntry,
            this, &MainWindow::onNewLogEntry);
}

// ... (Resto de on_pushButton_clicked y onProductionLineStatus igual) ...
// ... (omito código repetido por brevedad) ...

void MainWindow::on_pushButton_Start_clicked() {
    bool ok;
    int goal = QInputDialog::getInt(
        this,
        "Meta de producción",
        "¿Cuántos productos desea producir?",
        20, 1, 10000, 1,
        &ok
        );

    if (!ok) return;

    controller->setProductionGoal(goal);
    controller->startProduction();

    ui->label_Status->setText("Producción en marcha...");
    ui->label_Status->setStyleSheet("color: green; font-weight: bold;");
    ui->textEdit_Log->append(">>> Simulación iniciada <<<");

    metricsTimer->start(1000);
}

void MainWindow::on_pushButton_Stop_clicked() {
    controller->stopProduction();
    ui->label_Status->setText("Sistema detenido");
    ui->label_Status->setStyleSheet("color: red; font-weight: bold;");
    ui->textEdit_Log->append(">>> Simulación detenida <<<");
    metricsTimer->stop();
}

void MainWindow::on_pushButton_Pause_clicked() {
    controller->stopProduction();
    ui->label_Status->setText("Simulación pausada");
    ui->label_Status->setStyleSheet("color: orange; font-weight: bold;");
    ui->textEdit_Log->append(">>> Simulación pausada <<<");
}

void MainWindow::on_pushButton_Reset_clicked() {
    // Al resetear, limpiamos el log de la GUI, pero los datos JSON siguen en el archivo
    ui->textEdit_Log->clear();
    ui->label_ProductsProcessed->setText("Productos procesados: 0");
    productsProcessed = 0;
    ui->progressBar_Throughput->setValue(0);
    ui->label_Status->setText("Sistema reseteado");
    ui->label_Status->setStyleSheet("color: blue; font-weight: bold;");

    // Necesitamos recrear la línea después de un reset si queremos iniciar de nuevo:
    controller->setupProductionLine(5);
}

void MainWindow::onProductionLineStatus(const QString &msg) {
    ui->textEdit_Log->append(msg);
}

void MainWindow::onStationStatusUpdate(int stationId, const QString &status) {

    QString msg = QString("[Estación %1] %2").arg(stationId).arg(status);
    ui->textEdit_Log->append(msg);

    ui->tableWidget_Threads->setItem(stationId-1, 0,
                                     new QTableWidgetItem(QString("Estación %1").arg(stationId)));

    ui->tableWidget_Threads->setItem(stationId-1, 1,
                                     new QTableWidgetItem(status));

    updateStationVisual(stationId, status);
}

void MainWindow::onProductFinishedProcessing(const Product &product,
                                             const QString &stationName) {

    // Lógica para actualizar métricas cuando el producto llega a la última estación
    if (stationName.contains("Almacenamiento")) {
        int done = controller->getCompletedCount();
        int goal = controller->getTotalGoal();

        ui->label_ProductsProcessed->setText(
            QString("Productos procesados: %1 / %2").arg(done).arg(goal)
            );

        int percent = (goal > 0) ? (done * 100) / goal : 0;
        ui->progressBar_Throughput->setValue(percent);
    }

    // Log de avance de producto (para estados intermedios)
    QString msg = QString("[%1] completó producto ID:%2 (Estado:%3)")
                      .arg(stationName)
                      .arg(product.getId())
                      .arg(product.getCurrentState());

    ui->textEdit_Log->append(msg);
}

// NUEVA IMPLEMENTACIÓN: Muestra el registro guardado por el Logger
void MainWindow::onNewLogEntry(const QString& message) {
    QString logMsg = QString("💾 JSON LOG | %1").arg(message);
    ui->textEdit_Log->append(logMsg);
}


void MainWindow::updateMetrics() {

    int active = controller->getActiveThreadCount();
    ui->label_ThreadsActive->setText(
        QString("Hilos activos: %1").arg(active)
        );

    int done = controller->getCompletedCount();
    int goal = controller->getTotalGoal();

    ui->label_ProductsProcessed->setText(
        QString("Productos procesados: %1 / %2").arg(done).arg(goal)
        );

    int percent = (goal > 0) ? (done * 100) / goal : 0;
    ui->progressBar_Throughput->setValue(percent);

    // Actualización de uso del primer buffer (Buffer 0)
    int buffer0_usage = controller->getBufferUsage(0);
    ui->label_BufferUsage->setText(
        QString("Buffer 0 (Ensamblaje): %1%").arg(buffer0_usage)
        );
}


void MainWindow::updateStationVisual(int stationId, const QString &status) {
    // ... (El código de updateStationVisual es correcto y se mantiene igual) ...
    QLabel *label = nullptr;

    // Mapping de IDs (Asegúrate de que tus labels en el .ui coincidan con estos nombres)
    if (stationId == 1) label = ui->label_Assembler;
    else if (stationId == 2) label = ui->label_Tester;
    else if (stationId == 3) label = ui->label_Packager; // Corregí el mapeo según tu setupProductionLine
    else if (stationId == 4) label = ui->label_Labeler;   // Corregí el mapeo según tu setupProductionLine
    else if (stationId == 5) label = ui->label_Storage;

    if (!label) return;

    if (status.contains("Procesando"))
        label->setStyleSheet("background-color: yellow; font-weight: bold;");
    else if (status.contains("Detenida"))
        label->setStyleSheet("background-color: lightgray; font-weight: bold;");
    else if (status.contains("Esperando"))
        label->setStyleSheet("background-color: orange; font-weight: bold;");
    else
        label->setStyleSheet("background-color: lightgreen; font-weight: bold;");
}

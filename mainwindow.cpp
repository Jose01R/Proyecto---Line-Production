#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>

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
}

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
    ui->textEdit_Log->clear();

    ui->label_ProductsProcessed->setText("Productos procesados: 0");
    productsProcessed = 0;
    ui->progressBar_Throughput->setValue(0);

    ui->label_Status->setText("Sistema reseteado");
    ui->label_Status->setStyleSheet("color: blue; font-weight: bold;");
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

    if (stationName.contains("Almacenamiento")) {

        int done = controller->getCompletedCount();
        int goal = controller->getTotalGoal();

        ui->label_ProductsProcessed->setText(
            QString("Productos procesados: %1 / %2").arg(done).arg(goal)
            );

        int percent = (done * 100) / goal;
        ui->progressBar_Throughput->setValue(percent);
    }

    QString msg = QString("[%1] completó producto ID:%2 (Estado:%3)")
                      .arg(stationName)
                      .arg(product.getId())
                      .arg(product.getCurrentState());

    ui->textEdit_Log->append(msg);
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

    ui->label_BufferUsage->setText(
        QString("Storage: %1%").arg(percent)
        );
}


void MainWindow::updateStationVisual(int stationId, const QString &status) {

    QLabel *label = nullptr;

    if (stationId == 1) label = ui->label_Assembler;
    else if (stationId == 2) label = ui->label_Tester;
    else if (stationId == 3) label = ui->label_Packager;
    else if (stationId == 4) label = ui->label_Labeler;
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




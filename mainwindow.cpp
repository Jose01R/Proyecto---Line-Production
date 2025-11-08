#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , buffer1(new Buffer(10))
    , buffer2(new Buffer(10))
    , assembler(nullptr)
    , tester(nullptr)
    , productsProcessed(0)
{
    ui->setupUi(this);
    ui->label_Status->setText("Sistema detenido");
    ui->textEdit_Log->setReadOnly(true);

    // Crear estaciones
    assembler = new Assembler(1, buffer1, buffer2, this);
    tester = new Tester(2, buffer2, nullptr, this);


    setupConnections();

    // Timer para métricas
    metricsTimer = new QTimer(this);
    connect(metricsTimer, &QTimer::timeout, this, &MainWindow::updateMetrics);
}

MainWindow::~MainWindow() {
    assembler->stopStation();
    tester->stopStation();
    delete ui;
}

void MainWindow::setupConnections() {
    // Conectar señales de estaciones
    connect(assembler, &Station::stationStatusUpdate, this, &MainWindow::onStationStatusUpdate);
    connect(tester, &Station::stationStatusUpdate, this, &MainWindow::onStationStatusUpdate);

    connect(assembler, &Station::productFinishedProcessing, this, &MainWindow::onProductFinishedProcessing);
    connect(tester, &Station::productFinishedProcessing, this, &MainWindow::onProductFinishedProcessing);
}

void MainWindow::on_pushButton_Start_clicked() {
    ui->label_Status->setText("Producción en marcha...");
    ui->label_Status->setStyleSheet("color: green; font-weight: bold;");
    ui->textEdit_Log->append(">>> Simulación iniciada <<<");

    // 🔹 AGREGAR ESTOS PRODUCTOS INICIALES:
    for (int i = 1; i <= 5; ++i) {
        Product* p = new Product(i, "Electrodoméstico Genérico");
        buffer1->addProduct(p);
    }

    assembler->start();
    tester->start();
    metricsTimer->start(1000);
}


void MainWindow::on_pushButton_Stop_clicked() {
    assembler->stopStation();
    tester->stopStation();

    ui->label_Status->setText("Sistema detenido");
    ui->label_Status->setStyleSheet("color: red; font-weight: bold;");
    ui->textEdit_Log->append(">>> Simulación detenida <<<");

    metricsTimer->stop();
}

void MainWindow::on_pushButton_Pause_clicked() {
    assembler->stopStation();
    tester->stopStation();
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

void MainWindow::onStationStatusUpdate(int stationId, const QString &status) {
    QString msg = QString("[Estación %1] %2").arg(stationId).arg(status);
    ui->textEdit_Log->append(msg);

    // Actualizar color visual
    if (stationId == 1)
        updateStationVisual("Assembler", status);
    else if (stationId == 2)
        updateStationVisual("Tester", status);
}

void MainWindow::onProductFinishedProcessing(const Product &product, const QString &stationName) {
    productsProcessed++;
    QString msg = QString("[%1] completó producto ID:%2 (Estado:%3)")
                      .arg(stationName)
                      .arg(product.getId())
                      .arg(product.getCurrentState());
    ui->textEdit_Log->append(msg);

    ui->label_ProductsProcessed->setText(QString("Productos procesados: %1").arg(productsProcessed));
}

void MainWindow::updateStationVisual(const QString &stationName, const QString &status) {
    QLabel *label = nullptr;
    if (stationName.contains("Ensamblaje")) label = ui->label_Assembler;
    else if (stationName.contains("Inspección")) label = ui->label_Tester;
    else if (stationName.contains("Empaquetado")) label = ui->label_Packager;

    if (!label) return;

    if (status.contains("Procesando")) {
        label->setStyleSheet("background-color: yellow; border: 2px solid gray; border-radius: 8px; font-weight: bold;");
    } else if (status.contains("Detenida")) {
        label->setStyleSheet("background-color: lightgray; border: 2px solid gray; border-radius: 8px; font-weight: bold;");
    } else if (status.contains("Esperando")) {
        label->setStyleSheet("background-color: orange; border: 2px solid gray; border-radius: 8px; font-weight: bold;");
    } else {
        label->setStyleSheet("background-color: lightgreen; border: 2px solid gray; border-radius: 8px; font-weight: bold;");
    }
}

void MainWindow::updateMetrics() {
    // Actualiza hilos y throughput
    ui->label_ThreadsActive->setText("Hilos activos: 2");
    ui->progressBar_Throughput->setValue(qMin(productsProcessed * 5, 100));
}


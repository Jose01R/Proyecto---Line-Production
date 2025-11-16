#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QDateTime>
#include "logger.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , controller(new ProductionController(this))
    , metricsTimer(new QTimer(this))
    , productsProcessed(0)

    //
    // HILOS DE MANTENIMIENTO
    //
    , cleanThread(new GeneralCleanThreads(controller, this))
    , logsThread(new GeneralLogs(controller, this))
    , statsThread(new GeneralStats(controller, this))
{
    ui->setupUi(this);
    statsWindow = new StatsWindow(this);

    ui->label_Status->setText("Sistema detenido");
    ui->textEdit_Log->setReadOnly(true);
    ui->tableWidget_Threads->setRowCount(5);

    controller->setupProductionLine(5);

    setupConnections();

    connect(metricsTimer, &QTimer::timeout,
            this, &MainWindow::updateMetrics);

    //
    // INICIO DE HILOS
    //
    cleanThread->start();
    logsThread->start();
    statsThread->start();
}

MainWindow::~MainWindow() {

    cleanThread->stopSafely();
    cleanThread->wait();

    logsThread->stopSafely();
    logsThread->wait();

    statsThread->stopSafely();
    statsThread->wait();

    delete ui;
}

void MainWindow::setupConnections() {

    connect(controller, &ProductionController::updateStationStatus,
            this, &MainWindow::onStationStatusUpdate);

    connect(controller, &ProductionController::productStateChanged,
            this, &MainWindow::onProductFinishedProcessing);

    connect(controller, &ProductionController::productionLineStatus,
            this, &MainWindow::onProductionLineStatus);

    connect(controller->getLogger(), &Logger::newLogEntry,
            this, &MainWindow::onNewLogEntry);

    connect(ui->pushButton_History, &QPushButton::clicked,
            this, &MainWindow::openHistoryWindow);

    // ======================================
    // HILOS DE MANTENIMIENTO
    // ======================================
    connect(cleanThread, &GeneralThread::threadMessage,
            this, &MainWindow::handleThreadMessage);

    connect(logsThread, &GeneralThread::threadMessage,
            this, &MainWindow::handleThreadMessage);

    connect(statsThread, &GeneralThread::threadMessage,
            this, &MainWindow::handleThreadMessage);

    connect(cleanThread, &GeneralCleanThreads::requestSystemReset,
            this, &MainWindow::handleSystemResetRequest);

    connect(statsThread, &GeneralStats::statsDataReady,
            this, &MainWindow::handleStatsData);

    connect(ui->pushButton_Stats, &QPushButton::clicked, [=](){
        statsWindow->show();
        statsWindow->raise();
    });
}

//mensajje de los hilos
void MainWindow::handleThreadMessage(const QString& name,
                                     const QString& action,
                                     const QString& message)
{
    QString logMsg =
        QString("[%1] [HILO %2] %3: %4")
            .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
            .arg(name)
            .arg(action)
            .arg(message);

    ui->textEdit_Log->append(logMsg);
}

void MainWindow::handleSystemResetRequest() {

    metricsTimer->stop();
    controller->stopProduction();
    on_pushButton_Reset_clicked();

    ui->label_Status->setText("Reseteo forzado por mantenimiento");

    ui->textEdit_Log->append(
        QString("[%1] [MainWindow] Limpieza forzada ejecutada")
            .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
        );
}

void MainWindow::handleStatsData(const QString& statsJson) {
    statsWindow->updateChart(statsJson);

    ui->textEdit_Log->append(
        "<span style='color:#4CAF50;'>📊 Estadísticas actualizadas (ver ventana de estadísticas)</span>"
        );
}


//botones  princiipales

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

    ui->label_Status->setText("Producción en marcha");
    ui->label_Status->setStyleSheet("color: green; font-weight:bold;");

    ui->textEdit_Log->append(">>> Simulación iniciada <<<");
    metricsTimer->start(1000);
}

void MainWindow::on_pushButton_Stop_clicked() {

    controller->stopProduction();
    ui->label_Status->setText("Producción detenida");
    ui->label_Status->setStyleSheet("color: red; font-weight:bold;");

    ui->textEdit_Log->append(">>> Simulación detenida <<<");
    metricsTimer->stop();
}

void MainWindow::on_pushButton_Pause_clicked() {

    // NO estaba pausado entonces Pausar
    if (!controller->getPaused()) {

        controller->pauseProduction();

        ui->label_Status->setText("Producción pausada");
        ui->label_Status->setStyleSheet("color: orange; font-weight:bold;");
        ui->pushButton_Pause->setText("▶ Reanudar");

        ui->textEdit_Log->append(">>> Producción pausada <<<");

        return;
    }

    // SÍ estaba pausado entonces  Reanudar
    controller->resumeProduction();

    ui->label_Status->setText("Producción en marcha");
    ui->label_Status->setStyleSheet("color: green; font-weight:bold;");
    ui->pushButton_Pause->setText("⏸ Pausar");

    ui->textEdit_Log->append(">>> Producción reanudada <<<");
}

void MainWindow::on_pushButton_Reset_clicked() {

    ui->textEdit_Log->clear();
    ui->label_ProductsProcessed->setText("Productos procesados: 0");

    productsProcessed = 0;

    ui->progressBar_Throughput->setValue(0);
    ui->label_Status->setText("Sistema reseteado");
    ui->label_Status->setStyleSheet("color: blue; font-weight:bold;");

    controller->setupProductionLine(5);
}

//actualizacion de estaciones

void MainWindow::onStationStatusUpdate(int stationId, const QString &status) {

    QString msg = QString("[Estación %1] %2").arg(stationId).arg(status);
    ui->textEdit_Log->append(msg);

    ui->tableWidget_Threads->setItem(stationId-1, 0,
                                     new QTableWidgetItem(QString("Estación %1").arg(stationId)));

    ui->tableWidget_Threads->setItem(stationId-1, 1,
                                     new QTableWidgetItem(status));

    updateStationVisual(stationId, status);
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
        label->setStyleSheet("background-color: yellow; font-weight:bold;");

    else if (status.contains("Esperando"))
        label->setStyleSheet("background-color: orange; font-weight:bold;");

    else if (status.contains("Pausada"))
        label->setStyleSheet("background-color: lightblue; font-weight:bold;");

    else if (status.contains("Detenida"))
        label->setStyleSheet("background-color: lightgray; font-weight:bold;");

    else
        label->setStyleSheet("background-color: lightgreen; font-weight:bold;");
}



//finalizacion del prooducto
void MainWindow::onProductFinishedProcessing(const Product &product,
                                             const QString &stationName)
{
    if (stationName.contains("Almacenamiento")) {

        int done = controller->getCompletedCount();
        int goal = controller->getTotalGoal();

        ui->label_ProductsProcessed->setText(
            QString("Productos procesados: %1 / %2").arg(done).arg(goal)
            );

        ui->progressBar_Throughput->setValue(
            goal > 0 ? (done * 100) / goal : 0);
    }

    ui->textEdit_Log->append(
        QString("[%1] completó producto ID:%2 (Estado:%3)")
            .arg(stationName)
            .arg(product.getId())
            .arg(product.getCurrentState())
        );
}

//log json
void MainWindow::onNewLogEntry(const QString& message) {

    ui->textEdit_Log->append(
        QString("<span style='color:#888;'>💾 JSON LOG → %1</span>").arg(message)
        );
}

//metricas
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

    int buffer0 = controller->getBufferUsage(0);
    ui->label_BufferUsage->setText(
        QString("Buffer 0 (Ensamblaje): %1%").arg(buffer0)
        );
}


//historial
void MainWindow::openHistoryWindow() {
    HistoryWindow* hw = new HistoryWindow(controller->getLogger(), this);
    hw->exec();
}

void MainWindow::onProductionLineStatus(const QString &msg) {
    ui->textEdit_Log->append(msg);
}

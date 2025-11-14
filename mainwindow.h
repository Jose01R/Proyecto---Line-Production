#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "productioncontroller.h"
#include "logger.h" //Necesario para acceder a las señales del Logger
#include "historywindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Botones
    void on_pushButton_Start_clicked();
    void on_pushButton_Stop_clicked();
    void on_pushButton_Pause_clicked();
    void on_pushButton_Reset_clicked();

    // Señales recibidas desde controller
    void onStationStatusUpdate(int stationId, const QString &status);
    void onProductFinishedProcessing(const Product &product, const QString &stationName);
    void onProductionLineStatus(const QString& msg);

    //Slot para recibir la notificación de registro JSON desde el Logger
    void onNewLogEntry(const QString& message);

    // Métricas
    void updateMetrics();

private:
    Ui::MainWindow *ui;

    ProductionController *controller;
    QTimer *metricsTimer;
    int productsProcessed;
    void openHistoryWindow();
    void setupConnections();
    void updateStationVisual(int stationId, const QString &status);
};

#endif // MAINWINDOW_H

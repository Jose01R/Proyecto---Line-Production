#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "productioncontroller.h"
#include "logger.h" //Necesario para acceder a las señales del Logger
#include "historywindow.h"
#include "statswindow.h"


// ===============================================
// INCLUSIONES DE HILOS DE MANTENIMIENTO
// ===============================================
#include "generalthread.h"
#include "generalcleanthreads.h"
#include "generalstats.h"
#include "generallogs.h"
// Faltan GeneralLogs y GeneralStats (se incluirán luego)
// ===============================================

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

    // ===============================================
    // SLOTS PARA HILOS DE MANTENIMIENTO
    // ===============================================
    void handleThreadMessage(const QString& name, const QString& action, const QString& message);
    void handleSystemResetRequest();
    void handleStatsData(const QString& statsJson); //
    // ===============================================

private:
    Ui::MainWindow *ui;

    StatsWindow* statsWindow;
    ProductionController *controller;
    QTimer *metricsTimer;
    int productsProcessed;
    void openHistoryWindow();
    void setupConnections();
    void updateStationVisual(int stationId, const QString &status);

    // ===============================================
    // INSTANCIAS DE HILOS DE MANTENIMIENTO
    // Usamos punteros para gestionarlos en el heap y detenerlos en el destructor.
    // ===============================================
    GeneralCleanThreads *cleanThread;
    GeneralLogs *logsThread; // ¡NUEVO!
    GeneralStats *statsThread; // ¡NUEVO!
    // Agregaremos GeneralLogs y GeneralStats aquí en el futuro
    // ===============================================
};

#endif // MAINWINDOW_H

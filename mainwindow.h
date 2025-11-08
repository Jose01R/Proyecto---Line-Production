#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "buffer.h"
#include "assembler.h"
#include "tester.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Botones de la interfaz
    void on_pushButton_Start_clicked();
    void on_pushButton_Stop_clicked();
    void on_pushButton_Pause_clicked();
    void on_pushButton_Reset_clicked();

    // Señales de las estaciones
    void onStationStatusUpdate(int stationId, const QString &status);
    void onProductFinishedProcessing(const Product &product, const QString &stationName);

    // Actualización periódica de métricas
    void updateMetrics();

private:
    Ui::MainWindow *ui;

    // Elementos lógicos
    Buffer *buffer1;
    Buffer *buffer2;
    Assembler *assembler;
    Tester *tester;

    QTimer *metricsTimer;
    int productsProcessed;

    // Funciones auxiliares
    void setupConnections();
    void updateStationVisual(const QString &stationName, const QString &status);
};
#endif // MAINWINDOW_H


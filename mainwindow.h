#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QCloseEvent> // Necesario para manejar el guardado al cerrar
#include "productioncontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // Sobreescribe el evento de cierre para guardar datos y detener hilos
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    ProductionController *controller;
};

#endif // MAINWINDOW_H

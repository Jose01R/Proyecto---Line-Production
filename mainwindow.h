#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton> // Necesario si usas el Q_OBJECT con botones
#include "productioncontroller.h" // ¡Importante!

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    // DECLARACIÓN: El controlador es quien administra toda la lógica
    ProductionController *controller;
};

#endif // MAINWINDOW_H

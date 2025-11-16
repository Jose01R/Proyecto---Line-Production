/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.15
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_Controls;
    QHBoxLayout *layoutControls;
    QPushButton *pushButton_Start;
    QPushButton *pushButton_Pause;
    QPushButton *pushButton_Stop;
    QPushButton *pushButton_History;
    QPushButton *pushButton_Stats;
    QLabel *label_Status;
    QGroupBox *groupBox_Linea;
    QVBoxLayout *layoutLinea;
    QHBoxLayout *layoutStations;
    QLabel *label_Assembler;
    QLabel *label_Tester;
    QLabel *label_Packager;
    QLabel *label_Labeler;
    QLabel *label_Storage;
    QGroupBox *groupBox_Metrics;
    QVBoxLayout *layoutMetrics;
    QLabel *label_ProductsProcessed;
    QLabel *label_ThreadsActive;
    QLabel *label_BufferUsage;
    QProgressBar *progressBar_Throughput;
    QTableWidget *tableWidget_Threads;
    QGroupBox *groupBox_Logs;
    QVBoxLayout *layoutLogs;
    QTextEdit *textEdit_Log;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1000, 700);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox_Controls = new QGroupBox(centralwidget);
        groupBox_Controls->setObjectName(QString::fromUtf8("groupBox_Controls"));
        layoutControls = new QHBoxLayout(groupBox_Controls);
        layoutControls->setObjectName(QString::fromUtf8("layoutControls"));
        pushButton_Start = new QPushButton(groupBox_Controls);
        pushButton_Start->setObjectName(QString::fromUtf8("pushButton_Start"));
        pushButton_Start->setStyleSheet(QString::fromUtf8("background-color: lightgreen; font-weight: bold;"));

        layoutControls->addWidget(pushButton_Start);

        pushButton_Pause = new QPushButton(groupBox_Controls);
        pushButton_Pause->setObjectName(QString::fromUtf8("pushButton_Pause"));
        pushButton_Pause->setStyleSheet(QString::fromUtf8("background-color: khaki; font-weight: bold;"));

        layoutControls->addWidget(pushButton_Pause);

        pushButton_Stop = new QPushButton(groupBox_Controls);
        pushButton_Stop->setObjectName(QString::fromUtf8("pushButton_Stop"));
        pushButton_Stop->setStyleSheet(QString::fromUtf8("background-color: salmon; font-weight: bold;"));

        layoutControls->addWidget(pushButton_Stop);

        pushButton_History = new QPushButton(groupBox_Controls);
        pushButton_History->setObjectName(QString::fromUtf8("pushButton_History"));
        pushButton_History->setStyleSheet(QString::fromUtf8("font-weight: bold;"));

        layoutControls->addWidget(pushButton_History);

        pushButton_Stats = new QPushButton(groupBox_Controls);
        pushButton_Stats->setObjectName(QString::fromUtf8("pushButton_Stats"));
        pushButton_Stats->setStyleSheet(QString::fromUtf8("font-weight: bold;"));

        layoutControls->addWidget(pushButton_Stats);

        label_Status = new QLabel(groupBox_Controls);
        label_Status->setObjectName(QString::fromUtf8("label_Status"));
        label_Status->setAlignment(Qt::AlignCenter);
        label_Status->setStyleSheet(QString::fromUtf8("font-weight: bold; color: red;"));

        layoutControls->addWidget(label_Status);


        gridLayout->addWidget(groupBox_Controls, 0, 0, 1, 2);

        groupBox_Linea = new QGroupBox(centralwidget);
        groupBox_Linea->setObjectName(QString::fromUtf8("groupBox_Linea"));
        layoutLinea = new QVBoxLayout(groupBox_Linea);
        layoutLinea->setObjectName(QString::fromUtf8("layoutLinea"));
        layoutStations = new QHBoxLayout();
        layoutStations->setObjectName(QString::fromUtf8("layoutStations"));
        label_Assembler = new QLabel(groupBox_Linea);
        label_Assembler->setObjectName(QString::fromUtf8("label_Assembler"));
        label_Assembler->setAlignment(Qt::AlignCenter);
        label_Assembler->setMinimumSize(QSize(150, 80));
        label_Assembler->setStyleSheet(QString::fromUtf8("background-color: lightgray; border: 2px solid gray; border-radius: 8px; font-weight: bold;"));

        layoutStations->addWidget(label_Assembler);

        label_Tester = new QLabel(groupBox_Linea);
        label_Tester->setObjectName(QString::fromUtf8("label_Tester"));
        label_Tester->setAlignment(Qt::AlignCenter);
        label_Tester->setMinimumSize(QSize(150, 80));
        label_Tester->setStyleSheet(QString::fromUtf8("background-color: lightgray; border: 2px solid gray; border-radius: 8px; font-weight: bold;"));

        layoutStations->addWidget(label_Tester);

        label_Packager = new QLabel(groupBox_Linea);
        label_Packager->setObjectName(QString::fromUtf8("label_Packager"));
        label_Packager->setAlignment(Qt::AlignCenter);
        label_Packager->setMinimumSize(QSize(150, 80));
        label_Packager->setStyleSheet(QString::fromUtf8("background-color: lightgray; border: 2px solid gray; border-radius: 8px; font-weight: bold;"));

        layoutStations->addWidget(label_Packager);

        label_Labeler = new QLabel(groupBox_Linea);
        label_Labeler->setObjectName(QString::fromUtf8("label_Labeler"));
        label_Labeler->setAlignment(Qt::AlignCenter);
        label_Labeler->setMinimumSize(QSize(150, 80));
        label_Labeler->setStyleSheet(QString::fromUtf8("background-color: lightgray; border: 2px solid gray; border-radius: 8px; font-weight: bold;"));

        layoutStations->addWidget(label_Labeler);

        label_Storage = new QLabel(groupBox_Linea);
        label_Storage->setObjectName(QString::fromUtf8("label_Storage"));
        label_Storage->setAlignment(Qt::AlignCenter);
        label_Storage->setMinimumSize(QSize(150, 80));
        label_Storage->setStyleSheet(QString::fromUtf8("background-color: lightgray; border: 2px solid gray; border-radius: 8px; font-weight: bold;"));

        layoutStations->addWidget(label_Storage);


        layoutLinea->addLayout(layoutStations);


        gridLayout->addWidget(groupBox_Linea, 1, 0, 1, 1);

        groupBox_Metrics = new QGroupBox(centralwidget);
        groupBox_Metrics->setObjectName(QString::fromUtf8("groupBox_Metrics"));
        layoutMetrics = new QVBoxLayout(groupBox_Metrics);
        layoutMetrics->setObjectName(QString::fromUtf8("layoutMetrics"));
        label_ProductsProcessed = new QLabel(groupBox_Metrics);
        label_ProductsProcessed->setObjectName(QString::fromUtf8("label_ProductsProcessed"));

        layoutMetrics->addWidget(label_ProductsProcessed);

        label_ThreadsActive = new QLabel(groupBox_Metrics);
        label_ThreadsActive->setObjectName(QString::fromUtf8("label_ThreadsActive"));

        layoutMetrics->addWidget(label_ThreadsActive);

        label_BufferUsage = new QLabel(groupBox_Metrics);
        label_BufferUsage->setObjectName(QString::fromUtf8("label_BufferUsage"));

        layoutMetrics->addWidget(label_BufferUsage);

        progressBar_Throughput = new QProgressBar(groupBox_Metrics);
        progressBar_Throughput->setObjectName(QString::fromUtf8("progressBar_Throughput"));
        progressBar_Throughput->setValue(0);

        layoutMetrics->addWidget(progressBar_Throughput);

        tableWidget_Threads = new QTableWidget(groupBox_Metrics);
        if (tableWidget_Threads->columnCount() < 2)
            tableWidget_Threads->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_Threads->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_Threads->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget_Threads->setObjectName(QString::fromUtf8("tableWidget_Threads"));
        tableWidget_Threads->setRowCount(0);
        tableWidget_Threads->setColumnCount(2);

        layoutMetrics->addWidget(tableWidget_Threads);


        gridLayout->addWidget(groupBox_Metrics, 1, 1, 1, 1);

        groupBox_Logs = new QGroupBox(centralwidget);
        groupBox_Logs->setObjectName(QString::fromUtf8("groupBox_Logs"));
        layoutLogs = new QVBoxLayout(groupBox_Logs);
        layoutLogs->setObjectName(QString::fromUtf8("layoutLogs"));
        textEdit_Log = new QTextEdit(groupBox_Logs);
        textEdit_Log->setObjectName(QString::fromUtf8("textEdit_Log"));
        textEdit_Log->setReadOnly(true);
        textEdit_Log->setStyleSheet(QString::fromUtf8("background-color: #f7f7f7; font-family: Consolas, monospace;"));

        layoutLogs->addWidget(textEdit_Log);


        gridLayout->addWidget(groupBox_Logs, 2, 0, 1, 2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1000, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Simulaci\303\263n de L\303\255nea de Producci\303\263n", nullptr));
        groupBox_Controls->setTitle(QCoreApplication::translate("MainWindow", "Controles de Simulaci\303\263n", nullptr));
        pushButton_Start->setText(QCoreApplication::translate("MainWindow", "\342\226\266 Iniciar", nullptr));
        pushButton_Pause->setText(QCoreApplication::translate("MainWindow", "\342\217\270 Pausar", nullptr));
        pushButton_Stop->setText(QCoreApplication::translate("MainWindow", "\342\217\271 Detener", nullptr));
        pushButton_History->setText(QCoreApplication::translate("MainWindow", "\360\237\223\204 Historial", nullptr));
        pushButton_Stats->setText(QCoreApplication::translate("MainWindow", "\360\237\223\212 Grafico", nullptr));
        label_Status->setText(QCoreApplication::translate("MainWindow", "Estado: Sistema detenido", nullptr));
        groupBox_Linea->setTitle(QCoreApplication::translate("MainWindow", "L\303\255nea de Producci\303\263n", nullptr));
        label_Assembler->setText(QCoreApplication::translate("MainWindow", "\360\237\224\247 Ensamblaje", nullptr));
        label_Tester->setText(QCoreApplication::translate("MainWindow", "\360\237\247\252 Inspecci\303\263n", nullptr));
        label_Packager->setText(QCoreApplication::translate("MainWindow", "\360\237\223\246 Empaquetado", nullptr));
        label_Labeler->setText(QCoreApplication::translate("MainWindow", "\360\237\217\267\357\270\217 Etiquetado", nullptr));
        label_Storage->setText(QCoreApplication::translate("MainWindow", "\360\237\217\255 Almacenamiento", nullptr));
        groupBox_Metrics->setTitle(QCoreApplication::translate("MainWindow", "M\303\251tricas y Estad\303\255sticas", nullptr));
        label_ProductsProcessed->setText(QCoreApplication::translate("MainWindow", "Productos procesados: 0", nullptr));
        label_ThreadsActive->setText(QCoreApplication::translate("MainWindow", "Hilos activos: 0", nullptr));
        label_BufferUsage->setText(QCoreApplication::translate("MainWindow", "Uso de buffers: 0%", nullptr));
        progressBar_Throughput->setFormat(QCoreApplication::translate("MainWindow", "Rendimiento: %p%", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_Threads->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Hilo", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_Threads->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Estado", nullptr));
        groupBox_Logs->setTitle(QCoreApplication::translate("MainWindow", "Registro de Eventos", nullptr));
        textEdit_Log->setPlaceholderText(QCoreApplication::translate("MainWindow", "Mensajes de las estaciones aparecer\303\241n aqu\303\255...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

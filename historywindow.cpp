#include "historywindow.h"
#include "ui_historywindow.h"

HistoryWindow::HistoryWindow(Logger* logger, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::HistoryWindow),
    loggerRef(logger)
{
    ui->setupUi(this);

    loadHistory();

    connect(ui->buttonClose, &QPushButton::clicked,
            this, &HistoryWindow::on_buttonClose_clicked);
}

HistoryWindow::~HistoryWindow()
{
    delete ui;
}

void HistoryWindow::loadHistory()
{
    auto logs = loggerRef->getDailyLog();

    ui->tableHistory->setRowCount(logs.size());
    ui->tableHistory->setColumnCount(4);

    for (int i = 0; i < logs.size(); i++) {
        const auto& entry = logs[i];

        ui->tableHistory->setItem(i, 0, new QTableWidgetItem(QString::number(entry.productId)));
        ui->tableHistory->setItem(i, 1, new QTableWidgetItem(entry.productType));
        ui->tableHistory->setItem(i, 2, new QTableWidgetItem(entry.creationTime.toString("dd/MM hh:mm:ss")));
        ui->tableHistory->setItem(i, 3, new QTableWidgetItem(entry.completionTime.toString("dd/MM hh:mm:ss")));
    }
}

void HistoryWindow::on_buttonClose_clicked()
{
    close();
}


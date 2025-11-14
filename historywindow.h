#ifndef HISTORYWINDOW_H
#define HISTORYWINDOW_H

#include <QDialog>
#include "logger.h"

namespace Ui {
class HistoryWindow;
}

class HistoryWindow : public QDialog
{
    Q_OBJECT

public:
    explicit HistoryWindow(Logger* logger, QWidget *parent = nullptr);
    ~HistoryWindow();

private:
    Ui::HistoryWindow *ui;
    Logger* loggerRef;

    void loadHistory(); // carga los datos desde Logger

private slots:
    void on_buttonClose_clicked();
};

#endif // HISTORYWINDOW_H


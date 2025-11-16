#ifndef STATSWINDOW_H
#define STATSWINDOW_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

    class StatsWindow : public QDialog {
    Q_OBJECT

public:
    explicit StatsWindow(QWidget* parent = nullptr);

public slots:
    void updateChart(const QString& json);

private:
    QChartView* chartView;
};

#endif // STATSWINDOW_H

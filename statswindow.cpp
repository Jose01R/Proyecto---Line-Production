#include "statswindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QVBoxLayout>

StatsWindow::StatsWindow(QWidget* parent)
    : QDialog(parent),
    chartView(new QChartView)
{
    setWindowTitle("📊 Estadísticas del Sistema");
    setMinimumSize(600, 400);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
}

void StatsWindow::updateChart(const QString& json)
{
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    if (!doc.isObject()) return;

    QJsonObject obj = doc.object();

    // Datos principales
    int completed = obj.value("completed_products").toInt();
    int goal = obj.value("production_goal").toInt();
    int threads = obj.value("active_threads").toInt();

    // Buffers (ahora 0,1,2,3,4)
    int buf0 = obj.value("buffer_0_usage").toInt();
    int buf1 = obj.value("buffer_1_usage").toInt();
    int buf2 = obj.value("buffer_2_usage").toInt();
    int buf3 = obj.value("buffer_3_usage").toInt();
    int buf4 = obj.value("buffer_4_usage").toInt();

    // Set de barras
    QBarSet* set = new QBarSet("Valores");
    *set << completed
         << goal
         << threads
         << buf0
         << buf1
         << buf2
         << buf3
         << buf4;

    QBarSeries* series = new QBarSeries();
    series->append(set);

    // Categorías a mostrar
    QStringList categories = {
        "Completados",
        "Meta",
        "Hilos",
        "Buffer0",
        "Buffer1",
        "Buffer2",
        "Buffer3",
        "Buffer4"
    };

    // Crear gráfico
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Estadísticas en tiempo real");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Eje X
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Eje Y (dinámico según la meta o 100)
    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(0, qMax(goal, 100));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chartView->setChart(chart);
}


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <../../Kod/Styr_m/Styr_m/mapGUI/qcustomplot.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void fetchData();
    void drawPlots();

private:
    QVector<double> sensorVector0;
    QVector<double> sensorVector1;
    QVector<double> sensorVector2;
    QVector<double> sensorVector3;
    QVector<double> sensorVector4;
    QVector<double> sensorVector5;
    QVector<double> sensorVector6;
    QVector<double> sensorVector7;
    QVector<double> timeVector;
    QCustomPlot* customPlot0;
    QCustomPlot* customPlot1;
    QCustomPlot* customPlot2;
    QCustomPlot* customPlot3;
    QCustomPlot* customPlot4;
    QCustomPlot* customPlot5;
    QCustomPlot* customPlot6;
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H

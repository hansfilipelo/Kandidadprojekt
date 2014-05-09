#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QDialog>
#include "gui.h"
#include "qcustomplot.h"

class Gui;

namespace Ui {
class graphWindow;
}

class graphWindow : public QDialog
{
    Q_OBJECT

public:
    explicit graphWindow(QWidget *parent = 0);
    ~graphWindow();

private slots:
    void on_pushButton_pressed();

private:
    Ui::graphWindow *ui;

    void fetchData();
    void drawPlots();

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

};

#endif // GRAPHWINDOW_H

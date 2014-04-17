#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QVector>
#include <QTimer>
#include<algorithm>
#include<string.h>
#include<iostream>
#include<QGraphicsScene>
#include <QtAlgorithms>
#include <QtSerialPort/QtSerialPort>


class SerialPort;
class Order;
namespace Ui {
class Gui;
}

class Gui : public QMainWindow
{
    Q_OBJECT

public:
    void giveValues();
    explicit Gui(QWidget *parent = 0);
    ~Gui();
    void setupPlots();
    void updateSensorValues(int value1, int value2, int value3, int value4, int value5, int value6);
    void updatePlots();
    void updateMap();
    void insertRow(char inArray[25]);
    int startPort();
    int connectStatus;
    void labelSet(QString);
    char mapArea[32][25];

private slots:
    void on_speedSlider_valueChanged(int value);

    void on_upButton_pressed();

    void on_downButton_pressed();

    void on_leftButton_pressed();

    void on_rightButton_pressed();

    void on_speedSlider_sliderReleased();

    void updateTimeVector();

    void on_actionForward_triggered();

    void on_actionDown_triggered();

    void on_actionLeft_triggered();

    void on_actionRight_triggered();

    void on_actionSpeedUp_triggered();

    void on_actionSlowDown_triggered();

    void on_pushButton_clicked();

    void on_stopButton_pressed();

    void on_actionStop_triggered();

    void on_fetchButton_pressed();

    void on_setParameterButton_pressed();

private:
    int val = 0;
    Ui::Gui *ui;
    QVector<int> sensorVector1;
    QVector<int> sensorVector2;
    QVector<int> sensorVector3;
    QVector<int> sensorVector4;
    QVector<int> sensorVector5;
    QVector<int> sensorVector6;
    QVector<double> timeVector;
    QTimer *timer = new QTimer(this);
    QVector<double> testVector;
    QGraphicsScene* scene = new QGraphicsScene();
    Order* bluetooth;
    SerialPort* serPort;

    int speedMultiplier;
    //testvariabler
    char map[10][17]={{'U','U','U','U','U','U','U','F','U','E','E','R','E','E','E','E','U'},
                      {'U','U','U','E','U','U','U','F','U','E','E','R','E','E','E','E','U'},
                      {'U','U','E','E','U','U','U','F','U','E','E','R','E','E','E','E','U'},
                      {'U','U','U','E','U','U','U','F','U','E','E','R','E','E','E','E','U'},
                      {'F','U','U','E','U','U','U','F','U','E','E','R','E','E','E','E','U'},
                      {'U','U','U','E','U','U','U','F','U','E','E','R','E','E','E','E','U'},
                      {'U','R','U','E','E','U','U','F','U','E','E','R','E','E','E','E','U'},
                      {'U','U','U','U','E','U','U','F','U','E','E','R','E','E','E','E','U'},
                      {'U','U','U','U','U','U','U','F','U','E','E','R','E','E','E','E','U'},
                      {'U','U','U','U','U','U','U','F','U','E','E','R','E','E','E','E','U'}};
    char testArray[25] = {24,'M',9,'F','F','F','F','F','F','F','F','F','F','F','F','F','F','F','F','F',0,0,0,0,0};
};



#endif // UI_H

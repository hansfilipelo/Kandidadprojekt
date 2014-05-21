#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QVector>
#include <QTimer>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <QGraphicsScene>
#include <QtAlgorithms>
#include <QtSerialPort/QtSerialPort>
#include <graphwindow.h>

class graphWindow;
class SerialPort;
class Order;
namespace Ui {
class Gui;
}

class Gui : public QMainWindow
{
    Q_OBJECT

public:
    void giveValues(int);
    explicit Gui(QWidget *parent = 0);
    ~Gui();
    void setupPlots();
    void updateSensorValues(int value0,int value1, int value2, int value3, int value4, int value5, int value6,int value7);
    void updatePlots();
    void updateMap();
    void insertPathToMap();
    void mergePathArray();
    void insertRow(char inArray[27]);
    int startPort();
    int connectStatus;
    void labelSet(QString);
    char mapArea[32][27];
    char pathArraySum[100];
    char pathArrayOne[27];
    char pathArrayTwo[27];
    char pathArrayThree[27];
    char pathArrayFour[27];
    void saveToFile();

private slots:
    void on_speedSlider_valueChanged(int value);

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

    void on_actionSetParameter_triggered();

    void on_actionHalt_triggered();

    void on_saveDataButton_pressed();


    void on_actionReview_Data_triggered();

    void on_actionSave_Data_triggered();

    void on_btResetButton_pressed();

private:
    graphWindow* graph;
    QTime time;
    int val = 0;
    Ui::Gui *ui;
    QVector<double> sensorVector0;
    QVector<double> sensorVector1;
    QVector<double> sensorVector2;
    QVector<double> sensorVector3;
    QVector<double> sensorVector4;
    QVector<double> sensorVector5;
    QVector<double> sensorVector6;
    QVector<double> sensorVector7;
    QVector<double> timeVector;
    QTimer *timer = new QTimer(this);
    QVector<double> testVector;
    QGraphicsScene* scene = new QGraphicsScene();
    Order* bluetooth;
    SerialPort* serPort;

    int speedMultiplier;
    //testvariabler
    char map[32][27];
    char testArray[25] = {24,'M',9,'F','F','F','F','F','F','F','F','F','F','F','F','F','F','F','F','F',0,0,0,0,0};
};



#endif // UI_H

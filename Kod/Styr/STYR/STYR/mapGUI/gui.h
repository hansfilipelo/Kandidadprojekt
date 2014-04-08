#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QVector>

namespace Ui {
class Gui;
}

class Gui : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gui(QWidget *parent = 0);
    ~Gui();
    void setupPlots();
    void updateSensorValues(int value1, int value2, int value3, int value4, int value5, int value6);
    void updatePlots();



private slots:
    void on_speedSlider_valueChanged(int value);

    void on_upButton_pressed();

    void on_downButton_pressed();

    void on_leftButton_pressed();

    void on_rightButton_pressed();

private:
    Ui::Gui *ui;
    QVector<int> sensorVector1;
    QVector<int> sensorVector2;
    QVector<int> sensorVector3;
    QVector<int> sensorVector4;
    QVector<int> sensorVector5;
    QVector<int> sensorVector6;

};

#endif // UI_H

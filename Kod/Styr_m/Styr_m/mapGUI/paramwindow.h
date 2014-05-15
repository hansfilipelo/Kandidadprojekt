#ifndef PARAMWINDOW_H
#define PARAMWINDOW_H

#include <QDialog>
#include "../../../Kommunikation/serialreader/order.h"

class Order;

namespace Ui {
class ParamWindow;
}

class ParamWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ParamWindow(QWidget *parent = 0);
    ~ParamWindow();
    void setOrder(Order*);

private slots:
    void on_closeButton_pressed();

    void on_setParamButton_pressed();

    void on_setDefaultButton_pressed();

private:
    Ui::ParamWindow *ui;
    Order* Bluetooth;

    //Default parameters

    double defKp;
    double defKd;
    int defRef;
    int defTrimLeft;
    int defTrimRight;
    int defFwdRefLong;
    int defBwdRefLong;
    int defFwdRefShort;
    int defBwdRefShort;
    int defRightCornerFront;
    int defRightCornerBack;
    int defRightWallFront;
    int defRightWallBack;
    int defHaltAfterSection;
    double defKp2;



};

#endif // PARAMWINDOW_H

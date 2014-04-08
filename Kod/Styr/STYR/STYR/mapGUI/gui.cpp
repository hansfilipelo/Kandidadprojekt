#include "gui.h"
#include "ui_gui.h"

Gui::Gui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Gui)
{
    ui->setupUi(this);
    setupPlots();
    ui->speedPercent->setText(0);

}

Gui::~Gui()
{
    delete ui;
}

void Gui::on_speedSlider_valueChanged(int value)
{
   QString integer = QString::number(value);
   ui->speedPercent->setText(integer);


   //Bluetooth->sendSpeed(value)
}


void Gui::setupPlots(){
    ui->sensorPlot1->addGraph();
    ui->sensorPlot1->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot1->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot1->addGraph();
    ui->sensorPlot1->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot1->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->sensorPlot1->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);

    ui->sensorPlot2->addGraph();
    ui->sensorPlot2->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot2->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot2->addGraph();
    ui->sensorPlot2->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot2->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->sensorPlot2->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);

    ui->sensorPlot3->addGraph();
    ui->sensorPlot3->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot3->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot3->addGraph();
    ui->sensorPlot3->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot3->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->sensorPlot3->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);

    ui->sensorPlot4->addGraph();
    ui->sensorPlot4->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot4->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot4->addGraph();
    ui->sensorPlot4->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot4->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->sensorPlot4->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);

    ui->sensorPlot5->addGraph();
    ui->sensorPlot5->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot5->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot5->addGraph();
    ui->sensorPlot5->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot5->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->sensorPlot5->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);

    ui->sensorPlot6->addGraph();
    ui->sensorPlot6->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot6->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot6->addGraph();
    ui->sensorPlot6->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot6->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->sensorPlot6->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
}

void Gui::updateSensorValues(int value1, int value2, int value3, int value4, int value5, int value6){
    if (sensorVector1.size() == 50){
    sensorVector1.push_front(value1);
    sensorVector1.pop_back();
    }
    else{
        sensorVector1.push_front(value1);
    }
    if (sensorVector2.size() == 50){
    sensorVector2.push_front(value2);
    sensorVector2.pop_back();
    }
    else{
        sensorVector2.push_front(value2);
    }
    if (sensorVector3.size() == 50){
    sensorVector3.push_front(value3);
    sensorVector3.pop_back();
    }
    else{
        sensorVector3.push_front(value3);
    }
    if (sensorVector4.size() == 50){
    sensorVector4.push_front(value4);
    sensorVector4.pop_back();
    }
    else{
        sensorVector4.push_front(value4);
    }
    if (sensorVector5.size() == 50){
    sensorVector5.push_front(value5);
    sensorVector5.pop_back();
    }
    else{
        sensorVector5.push_front(value5);
    }
    if (sensorVector6.size() == 50){
    sensorVector6.push_front(value6);
    sensorVector6.pop_back();
    }
    else{
        sensorVector6.push_front(value6);
    }

}

void updatePlots(){


}


void Gui::on_upButton_pressed()
{
    //Bluetooth->sendUp();
}

void Gui::on_downButton_pressed()
{
    //Bluetooth->sendDown();
}

void Gui::on_leftButton_pressed()
{
    //Bluetooth->sendLeft();
}

void Gui::on_rightButton_pressed()
{
    //Bluetooth->sendRight();
}

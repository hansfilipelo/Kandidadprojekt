#include "gui.h"
#include "ui_gui.h"

Gui::Gui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Gui)
{
    ui->setupUi(this);
    setupPlots();
    ui->speedPercent->setText(0);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimeVector()));
    timeVector.insert(0,0);
    timer->start(1000);

}

Gui::~Gui()
{
    delete ui;
}

void Gui::updateTimeVector(){
    updatePlots();
    timeVector.push_front(timeVector.at(0)+1);
    if (timeVector.size()==10){
        timeVector.pop_back();
    }
    timer->start(1000);
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
    ui->sensorPlot1->xAxis->setRange(-1,1);
    ui->sensorPlot1->xAxis->setAutoTickStep(false);
    ui->sensorPlot1->xAxis->setTickStep(2);

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
    if (sensorVector1.size() == 10){
    sensorVector1.push_front(value1);
    sensorVector1.pop_back();
    }
    else{
        sensorVector1.push_front(value1);
    }
    if (sensorVector2.size() == 10){
    sensorVector2.push_front(value2);
    sensorVector2.pop_back();
    }
    else{
        sensorVector2.push_front(value2);
    }
    if (sensorVector3.size() == 10){
    sensorVector3.push_front(value3);
    sensorVector3.pop_back();
    }
    else{
        sensorVector3.push_front(value3);
    }
    if (sensorVector4.size() == 10){
    sensorVector4.push_front(value4);
    sensorVector4.pop_back();
    }
    else{
        sensorVector4.push_front(value4);
    }
    if (sensorVector5.size() == 10){
    sensorVector5.push_front(value5);
    sensorVector5.pop_back();
    }
    else{
        sensorVector5.push_front(value5);
    }
    if (sensorVector6.size() == 10){
    sensorVector6.push_front(value6);
    sensorVector6.pop_back();
    }
    else{
        sensorVector6.push_front(value6);
    }

}

void Gui::updatePlots(){



    ui->sensor1data->setText(QString::number(timeVector.at(0)));
    ui->sensorPlot1->graph(0)->setData(timeVector, testVector);
    ui->sensorPlot1->xAxis->setRange(timeVector.at(timeVector.size()-1),timeVector.at(0));
    ui->sensorPlot1->replot();
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

void Gui::on_speedSlider_sliderReleased(){
    int multiplier = 2.5;
    //Bluetooth->updateSpeed(ui->speedSlider->value()*multiplier);
}

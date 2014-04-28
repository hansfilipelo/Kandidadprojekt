#include "gui.h"
#include "ui_gui.h"
#include <qiodevice.h>
#include "../../../Kommunikation/serialreader/order.h"
#include "../../../Kommunikation/serialreader/serialport.h"
#include <QtSerialPort/QSerialPort>
#include <QtCore>
#include <QIODevice>
#include <math.h>
#include <iostream>
#include <chrono>
#include <thread>

Gui::Gui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Gui)
{
    ui->setupUi(this);
    setupPlots();
    ui->speedPercent->setText(0);
    /*connect(timer, SIGNAL(timeout()), this, SLOT(updateTimeVector()));
    timeVector.insert(0,0);
    timer->start(1000);
    */
    ui->mapView->setScene(scene);
    //alla keys förutom pilarna kräver manuell connection, bör rensas upp då alla slots inte behövs. Har atm flera slots som gör samma saker
    // via shortcuts samt knappar i UI:n
    new QShortcut(QKeySequence(Qt::Key_Space), this, SLOT(on_actionStop_triggered()));
    time.start();

}

Gui::~Gui()
{
    delete ui;
}

void Gui::labelSet(QString text){
    ui->label->setText(text);
}


int Gui::startPort(){

        QTextStream standardOutput(stdout);
        QTextStream standardInput(stdin);

        QSerialPort* serialPort = new QSerialPort;
        QString serialPortName = "FireFly-AA63-SPP";
        serialPort->setPortName(serialPortName);

        if (!serialPort->open(QIODevice::ReadWrite)) {
            standardOutput << QObject::tr("Failed to open port %1, error: %2").arg(serialPortName).arg(serialPort->errorString()) << endl;
            return 1;
        }

        int serialPortBaudRate = 115200; // kanske inte fungerar just nu
        if (!serialPort->setBaudRate(serialPortBaudRate)) {
            standardOutput << QObject::tr("Failed to set 115200 baud for port %1, error: %2").arg(serialPortName).arg(serialPort->errorString()) << endl;
            return 1;
        }

        if (!serialPort->setDataBits(QSerialPort::Data8)) {
            standardOutput << QObject::tr("Failed to set 8 data bits for port %1, error: %2").arg(serialPortName).arg(serialPort->errorString()) << endl;
            return 1;
        }

        if (!serialPort->setParity(QSerialPort::NoParity)) {
            standardOutput << QObject::tr("Failed to set no parity for port %1, error: %2").arg(serialPortName).arg(serialPort->errorString()) << endl;
            return 1;
        }

        if (!serialPort->setStopBits(QSerialPort::OneStop)) {
            standardOutput << QObject::tr("Failed to set 1 stop bit for port %1, error: %2").arg(serialPortName).arg(serialPort->errorString()) << endl;
            return 1;
        }

        if (!serialPort->setFlowControl(QSerialPort::NoFlowControl)) {
            standardOutput << QObject::tr("Failed to set no flow control for port %1, error: %2").arg(serialPortName).arg(serialPort->errorString()) << endl;
            return 1;
        }

        SerialPort* port = new SerialPort(serialPort);
        serPort = port;
        port->setGui(this);
        Order * order = new Order(port);
        bluetooth = order;
        return 0;
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
}


void Gui::setupPlots(){
    ui->sensorPlot0->addGraph();
    ui->sensorPlot0->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot0->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot0->addGraph();
    ui->sensorPlot0->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot0->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->sensorPlot0->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->sensorPlot0->xAxis->setRange(0,10);
    ui->sensorPlot0->yAxis->setRange(0,450);
    ui->sensorPlot0->xAxis->setAutoTickStep(false);
    ui->sensorPlot0->xAxis->setTickStep(2);
    ui->sensorPlot0->graph(0)->setData(sensorVector0,timeVector);

    ui->sensorPlot1->addGraph();
    ui->sensorPlot1->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot1->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot1->addGraph();
    ui->sensorPlot1->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot1->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->sensorPlot1->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->sensorPlot1->xAxis->setRange(0,10);
    ui->sensorPlot1->yAxis->setRange(0,450);
    ui->sensorPlot1->xAxis->setAutoTickStep(false);
    ui->sensorPlot1->xAxis->setTickStep(2);
    ui->sensorPlot1->graph(0)->setData(sensorVector1,timeVector);

    ui->sensorPlot2->addGraph();
    ui->sensorPlot2->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot2->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot2->addGraph();
    ui->sensorPlot2->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot2->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->sensorPlot2->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->sensorPlot2->graph(0)->setData(timeVector,sensorVector1);
    ui->sensorPlot2->yAxis->setRange(0,130);

    ui->sensorPlot3->addGraph();
    ui->sensorPlot3->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot3->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot3->addGraph();
    ui->sensorPlot3->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot3->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->sensorPlot3->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->sensorPlot3->graph(0)->setData(timeVector,sensorVector3);
    ui->sensorPlot3->yAxis->setRange(0,160);

    ui->sensorPlot4->addGraph();
    ui->sensorPlot4->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot4->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot4->addGraph();
    ui->sensorPlot4->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot4->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->sensorPlot4->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->sensorPlot4->graph(0)->setData(timeVector,sensorVector4);
    ui->sensorPlot4->yAxis->setRange(0,160);

    ui->sensorPlot5->addGraph();
    ui->sensorPlot5->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot5->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot5->addGraph();
    ui->sensorPlot5->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot5->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->sensorPlot5->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->sensorPlot5->graph(0)->setData(timeVector,sensorVector5);
    ui->sensorPlot5->yAxis->setRange(0,160);

    ui->sensorPlot6->addGraph();
    ui->sensorPlot6->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot6->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot6->addGraph();
    ui->sensorPlot6->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot6->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->sensorPlot6->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->sensorPlot6->graph(0)->setData(timeVector,sensorVector6);
    ui->sensorPlot6->yAxis->setRange(0,100);
}


//simply exists for initial testing purposes.
void Gui::giveValues(int inInt){
   for(int i = 0; i <= inInt; i++){
    int val0 = rand()%270;
    int val1 = rand()%270;
    int val2 = rand()%270;
    int val3 = rand()%270;
    int val4 = rand()%270;
    int val5 = rand()%270;
    int val6 = rand()%270;
    int val7 = rand()%270;

    updateSensorValues(val0,val1,val2,val3,val4,val5,val6,val7);

    std::cout << "Waiting for 750 ms" << std::endl;
    std::chrono::milliseconds dura( 750 );
    std::this_thread::sleep_for( dura );
    std::cout << "Waited 750 ms\n";
   }
}

void Gui::updateSensorValues(int value0, int value1, int value2, int value3, int value4, int value5, int value6, int value7){
    sensorVector0.push_back(value0);
    sensorVector1.push_back(value1);
    sensorVector2.push_back(value2);
    sensorVector3.push_back(value3);
    sensorVector4.push_back(value4);
    sensorVector5.push_back(value5);
    sensorVector6.push_back(value6);
    sensorVector7.push_back(value7);
    timeVector.push_back((double)time.elapsed()/1000);

    ui->sensorPlot0->graph(0)->setData(timeVector,sensorVector1);
    ui->sensorPlot0->xAxis->setRange(timeVector.last()-10,timeVector.last()+1);
    ui->sensorPlot0->replot();
    ui->sensor0data->setText(QString::number(sensorVector1.last()));

    ui->sensorPlot1->graph(0)->setData(timeVector,sensorVector1);
    ui->sensorPlot1->xAxis->setRange(timeVector.last()-10,timeVector.last()+1);
    ui->sensorPlot1->replot();
    ui->sensor1data->setText(QString::number(sensorVector1.last()));

    ui->sensorPlot2->graph(0)->setData(timeVector,sensorVector2);
    ui->sensorPlot2->xAxis->setRange(timeVector.last()-10,timeVector.last()+1);
    ui->sensorPlot2->replot();
    ui->sensor2data->setText(QString::number(sensorVector2.last()));

    ui->sensorPlot3->graph(0)->setData(timeVector,sensorVector3);
    ui->sensorPlot3->xAxis->setRange(timeVector.last()-10,timeVector.last()+1);
    ui->sensorPlot3->replot();
    ui->sensor3data->setText(QString::number(sensorVector3.last()));

    ui->sensorPlot4->graph(0)->setData(timeVector,sensorVector4);
    ui->sensorPlot4->xAxis->setRange(timeVector.last()-10,timeVector.last()+1);
    ui->sensorPlot4->replot();
    ui->sensor4data->setText(QString::number(sensorVector4.last()));

    ui->sensorPlot5->graph(0)->setData(timeVector,sensorVector5);
    ui->sensorPlot5->xAxis->setRange(timeVector.last()-10,timeVector.last()+1);
    ui->sensorPlot5->replot();
    ui->sensor5data->setText(QString::number(sensorVector5.last()));

    ui->sensorPlot6->graph(0)->setData(timeVector,sensorVector6);
    ui->sensorPlot6->xAxis->setRange(timeVector.last()-10,timeVector.last()+1);
    ui->sensorPlot6->replot();
    ui->sensor6data->setText(QString::number(sensorVector6.last()));
}

void Gui::updatePlots(){
    ui->sensor1data->setText(QString::number(timeVector.at(0)));
    ui->sensorPlot1->graph(0)->setData(timeVector, testVector);
    ui->sensorPlot1->xAxis->setRange(timeVector.at(timeVector.size()-1),timeVector.at(0));
    ui->sensorPlot1->replot();
}
void Gui::insertRow(char inArray[25])
{
    int row = inArray[2];
    for(int i=0; i < 17; i++ ){
        map[row][i] = inArray[i+3];
        std::cout << map[row][i] << std::endl;
    }

}
void Gui::updateMap(){
    qDeleteAll( scene->items());
    for (int rad = 0; rad < 10; rad++){
        for(int kol = 0; kol < 17; kol ++){
            QGraphicsRectItem*   temp = new QGraphicsRectItem(kol*20,rad*20,20,20);
            if(map[rad][kol]=='E'){
                temp->setBrush(Qt::green);
               scene->addItem(temp);
            }
            else if(map[rad][kol]=='U'){
                temp->setBrush(Qt::green);
                scene->addItem(temp);
            }

            else if(map[rad][kol]=='F'){
                temp->setBrush(Qt::red);
                scene->addItem(temp);
            }
            else if(map[rad][kol]=='R'){
                QGraphicsEllipseItem* robot = new QGraphicsEllipseItem(kol*20,rad*20,20,20);
                robot->setBrush(Qt::black);
                temp->setBrush(Qt::green);
                scene->addItem(temp);
                scene->addItem(robot);
            }
        }

    }
}




//void Gui::on_upButton_pressed()
//{
//    if(connectStatus){
//    bluetooth->forward(speedMultiplier);}
//}

//void Gui::on_downButton_pressed()
//{
//    if(connectStatus){
//    bluetooth->backward(speedMultiplier);}
//}

//void Gui::on_leftButton_pressed()
//{
//    if(connectStatus){
//    bluetooth->rotateLeft(speedMultiplier);}
//}

//void Gui::on_rightButton_pressed()
//{
//    if(connectStatus){
//    bluetooth->rotateRight(speedMultiplier);}
//}

void Gui::on_speedSlider_sliderReleased(){
   speedMultiplier = ui->speedSlider->value();
}

void Gui::on_actionForward_triggered()
{
    if(connectStatus){
    bluetooth->forward(speedMultiplier);}
}

void Gui::on_actionDown_triggered()
{
    if(connectStatus){
    bluetooth->backward(speedMultiplier);}
}

void Gui::on_actionLeft_triggered()
{
    if(connectStatus){
    bluetooth->rotateLeft(speedMultiplier);}
}

void Gui::on_actionRight_triggered()
{
      if(connectStatus){
      bluetooth->rotateRight(speedMultiplier);}
}

void Gui::on_stopButton_pressed()
{
    if(connectStatus){
        bluetooth->halt();
    }
}

void Gui::on_actionStop_triggered()
{
    if(connectStatus){
        bluetooth->halt();
    }
}

void Gui::on_actionSpeedUp_triggered()
{
    ui->speedSlider->setValue(ui->speedSlider->value()+10);
    speedMultiplier = ui->speedSlider->value();
}

void Gui::on_actionSlowDown_triggered()
{
    ui->speedSlider->setValue(ui->speedSlider->value()-10);
    speedMultiplier = ui->speedSlider->value();
}

void Gui::on_pushButton_clicked()
{
    this->startPort();
}


void Gui::on_fetchButton_pressed()
{
    if(connectStatus){
        bluetooth->fetchMap();
    }
}

void Gui::on_setParameterButton_pressed()
{
   double kp;
   double kd;
   int ref;

   kp=ui->doubleSpinBox->value();
   kd=ui->doubleSpinBox_2->value();
   ref=ui->spinBox->value();
   if(connectStatus){
   bluetooth->setControlParameters(kp, kd, ref);
}
}

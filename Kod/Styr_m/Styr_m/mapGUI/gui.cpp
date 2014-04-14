#include "gui.h"
#include "ui_gui.h"
#include <qiodevice.h>
#include "../../../Kommunikation/serialreader/order.h"
#include "../../../Kommunikation/serialreader/serialport.h"
#include <QtSerialPort/QSerialPort>
#include <QtCore>
#include <QIODevice>

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
    ui->mapView->setScene(scene);
    //alla keys förutom pilarna kräver manuell connection, bör rensas upp då alla slots inte behövs. Har atm flera slots som gör samma saker
    // via shortcuts samt knappar i UI:n
    new QShortcut(QKeySequence(Qt::Key_Space), this, SLOT(on_actionStop_triggered()));

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
           //else {ui->label->setText("Connected.");
         //  }

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
void Gui::insertRow(char inArray[25], char inMap[10][17])
{
    int row = inArray[2];
    for(int i=0; i < 17; i++ ){
        inMap[row][i] = inArray[i+3];
        std::cout << inMap[row][i] << std::endl;
    }

}
void Gui::updateMap(char inMap[10][17]){
    qDeleteAll( scene->items());
    for (int rad = 0; rad < 10; rad++){
        for(int kol = 0; kol < 17; kol ++){
            QGraphicsRectItem*   temp = new QGraphicsRectItem(kol*20,rad*20,20,20);
            if(inMap[rad][kol]=='E'){
                temp->setBrush(Qt::green);
               scene->addItem(temp);
            }
            else if(inMap[rad][kol]=='U'){
                temp->setBrush(Qt::green);
                scene->addItem(temp);
            }

            else if(inMap[rad][kol]=='F'){
                temp->setBrush(Qt::red);
                scene->addItem(temp);
            }
            else if(inMap[rad][kol]=='R'){
                QGraphicsEllipseItem* robot = new QGraphicsEllipseItem(kol*20,rad*20,20,20);
                robot->setBrush(Qt::black);
                temp->setBrush(Qt::green);
                scene->addItem(temp);
                scene->addItem(robot);
            }
        }

    }
}




void Gui::on_upButton_pressed()
{
    //testar min fkn
//    insertRow(testArray,map);
//    updateMap(map);
    if(connectStatus){
    bluetooth->forward();}
}

void Gui::on_downButton_pressed()
{
    if(connectStatus){
    bluetooth->backward();}
}

void Gui::on_leftButton_pressed()
{
    if(connectStatus){
    bluetooth->rotateLeft();}
}

void Gui::on_rightButton_pressed()
{
    if(connectStatus){
    bluetooth->rotateRight();}
}

void Gui::on_speedSlider_sliderReleased(){
   speedMultiplier = ui->speedSlider->value();
}

void Gui::on_actionForward_triggered()
{
    if(connectStatus){
    bluetooth->forward();}
}

void Gui::on_actionDown_triggered()
{
    if(connectStatus){
    bluetooth->backward();}
}

void Gui::on_actionLeft_triggered()
{
    if(connectStatus){
    bluetooth->rotateLeft();}
}

void Gui::on_actionRight_triggered()
{
      if(connectStatus){
      bluetooth->rotateRight();}
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
}

void Gui::on_actionSlowDown_triggered()
{
    ui->speedSlider->setValue(ui->speedSlider->value()-10);
}

void Gui::on_pushButton_clicked()
{
    this->startPort();
}





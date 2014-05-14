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


    parWindow = new ParamWindow;


    ui->speedPercent->setText(0);
    ui->mapView->setScene(scene);
    //Due to a bug space key needs to be slot-mapped manually.
    new QShortcut(QKeySequence(Qt::Key_Space), this, SLOT(on_actionStop_triggered()));
    time.start();
    int i = ui->speedSlider->value();
    QString s = QString::number(i);
    ui->speedPercent->setText(s);
    speedMultiplier = i;

    for(unsigned int i = 0; i<32; i++){
        char array[27] = {0,0,0,'u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u',0,0,0,0,0,0,0};
        memcpy(mapArea[i],array,27);
    }
    updateMap();


}

Gui::~Gui()
{
    delete ui;
}

void Gui::labelSet(QString text){
    ui->label->setText(text);

}

/*
 *  Saves the current sensordata to a file that is created in the users homefolder.
 */

void Gui::saveToFile()
{
    QDir Dir = QDir::homePath();
    QString qAbsolutePath = Dir.absolutePath();
    Dir.mkdir(qAbsolutePath +"/MapMaster");

    QString path = qAbsolutePath + "/MapMaster";
    QString filename = path +"/savedValues.txt";

    /* Try and open a file for output */
    QString outputFilename = filename;
    QFile outputFile(outputFilename);
    outputFile.open(QIODevice::WriteOnly);

    /* Check if opened OK */
    if(!outputFile.isOpen()){
        qDebug() << "- Error, unable to open" << outputFilename << "for output";
        return;
    }

    /* Point a QTextStream object at the file */
    QTextStream outStream(&outputFile);

    /* Write the line to the file */

    for(int i = 0; i < timeVector.size(); i++){
    outStream << timeVector.at(i) << '\n';
    outStream << sensorVector0.at(i) << '\n';
    outStream << sensorVector1.at(i) << '\n';
    outStream << sensorVector2.at(i) << '\n';
    outStream << sensorVector3.at(i) << '\n';
    outStream << sensorVector4.at(i) << '\n';
    outStream << sensorVector5.at(i) << '\n';
    outStream << sensorVector6.at(i) << '\n';
    outStream << sensorVector7.at(i) << '\n';
    }
    /* Close the file */
    outputFile.close();
}

void Gui::rowReceived()
{
    bluetooth->rowReceived();
}


/*
 *  Sets all options for the serialPort interface (bluetooth)
 *  and then passes the object on to the classes who need it.
 */

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

    int serialPortBaudRate = 115200;
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
    parWindow->setOrder(bluetooth);
    return 0;
}

/*
 *  Update the text for the slider when the slider changes.
 */

void Gui::on_speedSlider_valueChanged(int value)
{
    QString integer = QString::number(value);
    ui->speedPercent->setText(integer);
}



/*
 *  Plotting settings.
 */

void Gui::setupPlots(){
    ui->sensorPlot0->addGraph();
    ui->sensorPlot0->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot0->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot0->addGraph();
    ui->sensorPlot0->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot0->graph(0)->setLineStyle(QCPGraph::lsLine);
    //ui->sensorPlot0->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->sensorPlot0->xAxis->setRange(0,10);
    ui->sensorPlot0->yAxis->setRange(0,540);
    ui->sensorPlot0->xAxis->setAutoTickStep(false);
    ui->sensorPlot0->xAxis->setTickStep(2);
    ui->sensorPlot0->graph(0)->setData(sensorVector0,timeVector);

    ui->sensorPlot1->addGraph();
    ui->sensorPlot1->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot1->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot1->addGraph();
    ui->sensorPlot1->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot1->graph(0)->setLineStyle(QCPGraph::lsLine);
    //ui->sensorPlot1->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->sensorPlot1->xAxis->setRange(0,10);
    ui->sensorPlot1->yAxis->setRange(0,540);
    ui->sensorPlot1->xAxis->setAutoTickStep(false);
    ui->sensorPlot1->xAxis->setTickStep(2);
    ui->sensorPlot1->graph(0)->setData(sensorVector1,timeVector);

    ui->sensorPlot2->addGraph();
    ui->sensorPlot2->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot2->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot2->addGraph();
    ui->sensorPlot2->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot2->graph(0)->setLineStyle(QCPGraph::lsLine);
   // ui->sensorPlot2->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->sensorPlot2->graph(0)->setData(timeVector,sensorVector1);
    ui->sensorPlot2->yAxis->setRange(0,130);
    ui->sensorPlot2->xAxis->setRange(0,10);
    ui->sensorPlot2->xAxis->setAutoTickStep(false);
    ui->sensorPlot2->xAxis->setTickStep(2);

    ui->sensorPlot3->addGraph();
    ui->sensorPlot3->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot3->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot3->addGraph();
    ui->sensorPlot3->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot3->graph(0)->setLineStyle(QCPGraph::lsLine);
    //ui->sensorPlot3->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->sensorPlot3->graph(0)->setData(timeVector,sensorVector3);
    ui->sensorPlot3->yAxis->setRange(0,160);
    ui->sensorPlot3->xAxis->setAutoTickStep(false);
    ui->sensorPlot3->xAxis->setTickStep(2);
    

    ui->sensorPlot4->addGraph();
    ui->sensorPlot4->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot4->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot4->addGraph();
    ui->sensorPlot4->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot4->graph(0)->setLineStyle(QCPGraph::lsLine);
   // ui->sensorPlot4->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->sensorPlot4->graph(0)->setData(timeVector,sensorVector4);
    ui->sensorPlot4->yAxis->setRange(0,160);
    ui->sensorPlot4->xAxis->setAutoTickStep(false);
    ui->sensorPlot4->xAxis->setTickStep(2);


    ui->sensorPlot5->addGraph();
    ui->sensorPlot5->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot5->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot5->addGraph();
    ui->sensorPlot5->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot5->graph(0)->setLineStyle(QCPGraph::lsLine);
    //ui->sensorPlot5->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->sensorPlot5->graph(0)->setData(timeVector,sensorVector5);
    ui->sensorPlot5->yAxis->setRange(0,160);
    ui->sensorPlot5->xAxis->setAutoTickStep(false);
    ui->sensorPlot5->xAxis->setTickStep(2);


    ui->sensorPlot6->addGraph();
    ui->sensorPlot6->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot6->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot6->addGraph();
    ui->sensorPlot6->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot6->graph(0)->setLineStyle(QCPGraph::lsLine);
    //ui->sensorPlot6->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->sensorPlot6->graph(0)->setData(timeVector,sensorVector6);
    ui->sensorPlot6->yAxis->setRange(0,160);
    ui->sensorPlot6->xAxis->setAutoTickStep(false);
    ui->sensorPlot6->xAxis->setTickStep(2);

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

/*
 *  Insert the sensorvalues and then update the plots. 
 *  Also puts a text label next to the plot with the exact value
 *  of the latest element.
 */

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

    ui->sensorPlot0->graph(0)->setData(timeVector,sensorVector0);
    ui->sensorPlot0->xAxis->setRange(timeVector.last()-10,timeVector.last()+1);
    ui->sensorPlot0->replot();
    ui->sensor0data->setText(QString::number(sensorVector0.last()));

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

/*
 *  Iterates through mapArea and draws the map accordingly. 
 *  e = explored. u = unknown. f = fire. c = closed. r = robot.
 */


void Gui::updateMap(){
    qDeleteAll( scene->items());
    for (int rad = 0; rad < 32; rad++){
        for(int kol = 3; kol < 20; kol ++){
            QGraphicsRectItem*   temp = new QGraphicsRectItem(31-(kol-3)*15,rad*15,15,15);
            if(mapArea[rad][kol]=='e'){
                temp->setBrush(Qt::green);
                scene->addItem(temp);
            }
            else if(mapArea[rad][kol]=='u'){
                temp->setBrush(Qt::white);
                scene->addItem(temp);
            }

            else if(mapArea[rad][kol]=='f'){
                temp->setBrush(Qt::red);
                scene->addItem(temp);
            }
            else if(mapArea[rad][kol]=='c'){
                temp->setBrush(Qt::lightGray);
                scene->addItem(temp);
            }
            else if(mapArea[rad][kol]=='r'){
                QGraphicsEllipseItem* robot = new QGraphicsEllipseItem(31-(kol-3)*15,rad*15,15,15);
                robot->setBrush(Qt::black);
                temp->setBrush(Qt::green);
                scene->addItem(temp);
                scene->addItem(robot);
            }
            else{
                std::cout << mapArea[rad][kol] << std::endl;
            }
        }

    }
}

/*
 *  All functions below are functions that trigger on certain events.
 *  like speedSlider_sliderReleased, that runs when the slider is released.
 */

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

void Gui::on_actionHalt_triggered()
{
    if(connectStatus){
        bluetooth->autonom(speedMultiplier);
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
    if(connectStatus){
        parWindow->show();
    }
}

void Gui::on_actionSetParameter_triggered()
{
    on_setParameterButton_pressed();
}

void Gui::on_actionReview_Data_triggered()
{
    if (graph != nullptr){
        delete graph;
    }
    graph = new graphWindow;
    graph->show();
}

void Gui::on_actionSave_Data_triggered()
{
    saveToFile();
}

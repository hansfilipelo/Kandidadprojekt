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
    ui->mapView->setScene(scene);
    //alla keys förutom pilarna kräver manuell connection, bör rensas upp då alla slots inte behövs. Har atm flera slots som gör samma saker
    // via shortcuts samt knappar i UI:n
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

    /* Check it opened OK */
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
    ui->sensorPlot1->xAxis->setAutoTickStep(false);
    ui->sensorPlot1->xAxis->setTickStep(2);

    ui->sensorPlot3->addGraph();
    ui->sensorPlot3->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot3->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot3->addGraph();
    ui->sensorPlot3->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot3->graph(0)->setLineStyle(QCPGraph::lsLine);
    //ui->sensorPlot3->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->sensorPlot3->graph(0)->setData(timeVector,sensorVector3);
    ui->sensorPlot3->yAxis->setRange(0,160);

    ui->sensorPlot4->addGraph();
    ui->sensorPlot4->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot4->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot4->addGraph();
    ui->sensorPlot4->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot4->graph(0)->setLineStyle(QCPGraph::lsLine);
   // ui->sensorPlot4->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->sensorPlot4->graph(0)->setData(timeVector,sensorVector4);
    ui->sensorPlot4->yAxis->setRange(0,160);

    ui->sensorPlot5->addGraph();
    ui->sensorPlot5->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot5->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot5->addGraph();
    ui->sensorPlot5->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot5->graph(0)->setLineStyle(QCPGraph::lsLine);
    //ui->sensorPlot5->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->sensorPlot5->graph(0)->setData(timeVector,sensorVector5);
    ui->sensorPlot5->yAxis->setRange(0,160);

    ui->sensorPlot6->addGraph();
    ui->sensorPlot6->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot6->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->sensorPlot6->addGraph();
    ui->sensorPlot6->graph(0)->setPen(QPen(Qt::blue));
    ui->sensorPlot6->graph(0)->setLineStyle(QCPGraph::lsLine);
    //ui->sensorPlot6->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->sensorPlot6->graph(0)->setData(timeVector,sensorVector6);
    ui->sensorPlot6->yAxis->setRange(0,160);
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
    for (int rad = 0; rad < 32; rad++){
        for(int kol = 3; kol < 20; kol ++){
            QGraphicsRectItem*   temp = new QGraphicsRectItem((kol-3)*10,(31-rad)*10,10,10);
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
                QGraphicsEllipseItem* robot = new QGraphicsEllipseItem((kol-3)*10,(31-rad)*10,10,10);
                robot->setBrush(Qt::black);
                temp->setBrush(Qt::green);
                scene->addItem(temp);
                scene->addItem(robot);
            }
            else if(mapArea[rad][kol]=='t'){
                temp->setBrush(Qt::yellow);
                scene->addItem(temp);
            }
            else{
                std::cout << mapArea[rad][kol] << std::endl;
            }
        }

    }
}

void Gui::insertPathToMap()
{
    //defines start coordinates
    int x = 4;
    int y = 16;
    int size = pathArraySum[0];
    for(int i=1; i < size+1; i++){

        char c = pathArraySum[i];
        std::cout<<"direction:"<<c<<std::endl;
        if(c == 'f'){
            x++;
            mapArea[y][x] = 't';
        }
        else if(c == 'r'){
            y--;
            mapArea[y][x] = 't';
        }
        else if(c == 'b'){
            x--;
            mapArea[y][x] = 't';
        }
        else if(c == 'l'){
            y++;
            mapArea[y][x] = 't';
        }
    }
}

void Gui::mergePathArray()
{
    pathArraySum[0]=pathArrayOne[3];
    for(int i=1; i<21; i++){
        pathArraySum[i]=pathArrayOne[i+3];
        pathArraySum[20+i]=pathArrayTwo[i+3];
        pathArraySum[40+i]=pathArrayThree[i+3];
        pathArraySum[60+i]=pathArrayFour[i+3];
    }
}

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
    double kp;
    double kd;
    int ref;
    int trimLeft;
    int trimRight;

    trimLeft = ui->trimValueLeft->value();
    trimRight = ui->trimValueRight->value();
    kp=ui->doubleSpinBox->value();
    kd=ui->doubleSpinBox_2->value();
    ref=ui->spinBox->value();
    if(connectStatus){
        bluetooth->setControlParameters(kp, kd, ref,trimLeft,trimRight);
    }
}

void Gui::on_actionSetParameter_triggered()
{
    on_setParameterButton_pressed();
}

void Gui::on_saveDataButton_pressed()
{
    saveToFile();
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

void Gui::on_btResetButton_pressed()
{
    bluetooth->btReset();
}

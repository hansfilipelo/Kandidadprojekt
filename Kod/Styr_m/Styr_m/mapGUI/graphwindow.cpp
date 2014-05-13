#include "graphwindow.h"
#include "ui_graphwindow.h"


/*
 *  Construct the plots, fetch data from file then draw the plots.
 */


graphWindow::graphWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::graphWindow)
{
    ui->setupUi(this);

    customPlot0 = ui->customPlot0;
    customPlot1 = ui->customPlot1;
    customPlot2 = ui->customPlot2;
    customPlot3 = ui->customPlot3;
    customPlot4 = ui->customPlot4;
    customPlot5 = ui->customPlot5;
    customPlot6 = ui->customPlot6;

    fetchData();
    drawPlots();
}

graphWindow::~graphWindow()
{
    delete ui;
}

void graphWindow::on_pushButton_pressed()
{
    this->close();
}

/*
 *  Open the file where the saved data is stored.
 *  read out all data to each data container. 
 *  lastly close the file.
 */


void graphWindow::fetchData(){

    QDir Dir = QDir::homePath();
    QString qAbsolutePath = Dir.absolutePath();
    Dir.mkdir(qAbsolutePath +"/MapMaster");

    QString path = qAbsolutePath + "/MapMaster";
    QString filename = path +"/savedValues.txt";
    //read the data from file.
    QString outputFilename = filename;
    QFile outputFile(outputFilename);
    outputFile.open(QIODevice::ReadOnly);

    if(!outputFile.isOpen()){
        qDebug() << "- Error, unable to open" << outputFilename << "for output";
        return;
    }
    QTextStream inStream(&outputFile);

    while(!inStream.atEnd()){
        timeVector.push_back(inStream.readLine().toDouble());
        sensorVector0.push_back(inStream.readLine().toDouble());
        sensorVector1.push_back(inStream.readLine().toDouble());
        sensorVector2.push_back(inStream.readLine().toDouble());
        sensorVector3.push_back(inStream.readLine().toDouble());
        sensorVector4.push_back(inStream.readLine().toDouble());
        sensorVector5.push_back(inStream.readLine().toDouble());
        sensorVector6.push_back(inStream.readLine().toDouble());
        sensorVector7.push_back(inStream.readLine().toDouble());
    }

    outputFile.close();
}

void graphWindow::drawPlots()
{

    customPlot0->addGraph();
    customPlot0->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    customPlot0->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));

    customPlot0->xAxis2->setVisible(true);
    customPlot0->xAxis2->setTickLabels(false);
    customPlot0->yAxis2->setVisible(true);
    customPlot0->yAxis2->setTickLabels(false);
    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(customPlot0->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot0->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot0->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot0->yAxis2, SLOT(setRange(QCPRange)));
    // pass data points to graphs:
    customPlot0->graph(0)->setData(timeVector, sensorVector0);
    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    customPlot0->graph(0)->rescaleAxes();

    customPlot0->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot0->replot();
    
    
    //--------------------------------------------------------------//
    //      Below here the pattern only repeats for each sensor     //
    //--------------------------------------------------------------//


    customPlot1->addGraph();
    customPlot1->graph(0)->setPen(QPen(Qt::blue));
    
    customPlot1->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    
    
    customPlot1->xAxis2->setVisible(true);
    customPlot1->xAxis2->setTickLabels(false);
    customPlot1->yAxis2->setVisible(true);
    customPlot1->yAxis2->setTickLabels(false);
    
    
    connect(customPlot1->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot1->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot1->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot1->yAxis2, SLOT(setRange(QCPRange)));
    
    customPlot1->graph(0)->setData(timeVector, sensorVector1);
    
    
    customPlot1->graph(0)->rescaleAxes();

    customPlot1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot1->replot();

    

    customPlot2->addGraph();
    customPlot2->graph(0)->setPen(QPen(Qt::blue));
    
    customPlot2->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    
    customPlot2->xAxis2->setVisible(true);
    customPlot2->xAxis2->setTickLabels(false);
    customPlot2->yAxis2->setVisible(true);
    customPlot2->yAxis2->setTickLabels(false);

    
    connect(customPlot2->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot2->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot2->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot2->yAxis2, SLOT(setRange(QCPRange)));

    customPlot2->graph(0)->setData(timeVector, sensorVector2);

    
    customPlot2->graph(0)->rescaleAxes();

    customPlot2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot2->replot();

    
    customPlot3->addGraph();
    customPlot3->graph(0)->setPen(QPen(Qt::blue));
    
    customPlot3->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    
    customPlot3->xAxis2->setVisible(true);
    customPlot3->xAxis2->setTickLabels(false);
    customPlot3->yAxis2->setVisible(true);
    customPlot3->yAxis2->setTickLabels(false);

    
    connect(customPlot3->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot3->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot3->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot3->yAxis2, SLOT(setRange(QCPRange)));

    
    customPlot3->graph(0)->setData(timeVector, sensorVector3);

    
    customPlot3->graph(0)->rescaleAxes();

    customPlot3->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot3->replot();
    
    customPlot4->addGraph();
    customPlot4->graph(0)->setPen(QPen(Qt::blue));
    
    customPlot4->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    
    customPlot4->xAxis2->setVisible(true);
    customPlot4->xAxis2->setTickLabels(false);
    customPlot4->yAxis2->setVisible(true);
    customPlot4->yAxis2->setTickLabels(false);

    connect(customPlot4->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot4->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot4->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot4->yAxis2, SLOT(setRange(QCPRange)));

    customPlot4->graph(0)->setData(timeVector, sensorVector4);

    customPlot4->graph(0)->rescaleAxes();

    customPlot4->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot4->replot();

    customPlot5->addGraph();
    customPlot5->graph(0)->setPen(QPen(Qt::blue));
    
    customPlot5->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    customPlot5->xAxis2->setVisible(true);
    customPlot5->xAxis2->setTickLabels(false);
    customPlot5->yAxis2->setVisible(true);
    customPlot5->yAxis2->setTickLabels(false);

    connect(customPlot5->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot5->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot5->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot5->yAxis2, SLOT(setRange(QCPRange)));

    customPlot5->graph(0)->setData(timeVector, sensorVector5);

    customPlot5->graph(0)->rescaleAxes();

    customPlot5->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot5->replot();
    
    customPlot6->addGraph();
    customPlot6->graph(0)->setPen(QPen(Qt::blue));
    customPlot6->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    customPlot6->xAxis2->setVisible(true);
    customPlot6->xAxis2->setTickLabels(false);
    customPlot6->yAxis2->setVisible(true);
    customPlot6->yAxis2->setTickLabels(false);
    connect(customPlot6->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot6->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot6->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot6->yAxis2, SLOT(setRange(QCPRange)));
    customPlot6->graph(0)->setData(timeVector, sensorVector6);
 
    customPlot6->graph(0)->rescaleAxes();

    customPlot6->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot6->replot();

}

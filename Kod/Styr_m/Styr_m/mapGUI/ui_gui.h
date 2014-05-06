/********************************************************************************
** Form generated from reading UI file 'gui.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUI_H
#define UI_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_Gui
{
public:
    QAction *actionForward;
    QAction *actionDown;
    QAction *actionLeft;
    QAction *actionRight;
    QAction *actionSpeedUp;
    QAction *actionSlowDown;
    QAction *actionStop;
    QAction *actionSetParameter;
    QAction *actionHalt;
    QWidget *centralWidget;
    QGraphicsView *mapView;
    QSlider *speedSlider;
    QLabel *sensor1;
    QLabel *sensor1data;
    QLabel *sensor2;
    QLabel *sensor2data;
    QLabel *sensor3;
    QLabel *sensor3data;
    QLabel *sensor4;
    QLabel *sensor4data;
    QLabel *sensor5;
    QLabel *sensor5data;
    QLabel *sensor6data;
    QLabel *sensor6;
    QLabel *speedLabel;
    QLabel *speedPercent;
    QCustomPlot *sensorPlot1;
    QCustomPlot *sensorPlot2;
    QCustomPlot *sensorPlot3;
    QCustomPlot *sensorPlot4;
    QCustomPlot *sensorPlot5;
    QCustomPlot *sensorPlot6;
    QLabel *label;
    QPushButton *pushButton;
    QPushButton *fetchButton;
    QLabel *kpLabel;
    QLabel *kdLabel;
    QLabel *refLabel;
    QDoubleSpinBox *doubleSpinBox;
    QDoubleSpinBox *doubleSpinBox_2;
    QSpinBox *spinBox;
    QLabel *pdControlLabel;
    QPushButton *setParameterButton;
    QCustomPlot *sensorPlot0;
    QLabel *sensor0;
    QLabel *sensor0data;
    QPushButton *temp90Button;
    QLabel *trimLeftLabel;
    QLabel *trimRightLabel;
    QSpinBox *trimValueLeft;
    QSpinBox *trimValueRight;
    QMenuBar *menuBar;
    QMenu *menuCommands;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Gui)
    {
        if (Gui->objectName().isEmpty())
            Gui->setObjectName(QStringLiteral("Gui"));
        Gui->resize(871, 806);
        actionForward = new QAction(Gui);
        actionForward->setObjectName(QStringLiteral("actionForward"));
        actionDown = new QAction(Gui);
        actionDown->setObjectName(QStringLiteral("actionDown"));
        actionLeft = new QAction(Gui);
        actionLeft->setObjectName(QStringLiteral("actionLeft"));
        actionRight = new QAction(Gui);
        actionRight->setObjectName(QStringLiteral("actionRight"));
        actionSpeedUp = new QAction(Gui);
        actionSpeedUp->setObjectName(QStringLiteral("actionSpeedUp"));
        actionSlowDown = new QAction(Gui);
        actionSlowDown->setObjectName(QStringLiteral("actionSlowDown"));
        actionStop = new QAction(Gui);
        actionStop->setObjectName(QStringLiteral("actionStop"));
        actionSetParameter = new QAction(Gui);
        actionSetParameter->setObjectName(QStringLiteral("actionSetParameter"));
        actionHalt = new QAction(Gui);
        actionHalt->setObjectName(QStringLiteral("actionHalt"));
        centralWidget = new QWidget(Gui);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        mapView = new QGraphicsView(centralWidget);
        mapView->setObjectName(QStringLiteral("mapView"));
        mapView->setGeometry(QRect(30, 30, 391, 351));
        mapView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        mapView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        speedSlider = new QSlider(centralWidget);
        speedSlider->setObjectName(QStringLiteral("speedSlider"));
        speedSlider->setGeometry(QRect(220, 500, 160, 22));
        speedSlider->setMaximum(100);
        speedSlider->setValue(50);
        speedSlider->setOrientation(Qt::Horizontal);
        sensor1 = new QLabel(centralWidget);
        sensor1->setObjectName(QStringLiteral("sensor1"));
        sensor1->setGeometry(QRect(431, 60, 111, 20));
        sensor1data = new QLabel(centralWidget);
        sensor1data->setObjectName(QStringLiteral("sensor1data"));
        sensor1data->setGeometry(QRect(450, 80, 81, 20));
        sensor2 = new QLabel(centralWidget);
        sensor2->setObjectName(QStringLiteral("sensor2"));
        sensor2->setGeometry(QRect(430, 180, 111, 20));
        sensor2data = new QLabel(centralWidget);
        sensor2data->setObjectName(QStringLiteral("sensor2data"));
        sensor2data->setGeometry(QRect(450, 200, 81, 20));
        sensor3 = new QLabel(centralWidget);
        sensor3->setObjectName(QStringLiteral("sensor3"));
        sensor3->setGeometry(QRect(41, 660, 111, 20));
        sensor3data = new QLabel(centralWidget);
        sensor3data->setObjectName(QStringLiteral("sensor3data"));
        sensor3data->setGeometry(QRect(60, 680, 81, 19));
        sensor4 = new QLabel(centralWidget);
        sensor4->setObjectName(QStringLiteral("sensor4"));
        sensor4->setGeometry(QRect(431, 420, 111, 20));
        sensor4data = new QLabel(centralWidget);
        sensor4data->setObjectName(QStringLiteral("sensor4data"));
        sensor4data->setGeometry(QRect(450, 440, 81, 20));
        sensor5 = new QLabel(centralWidget);
        sensor5->setObjectName(QStringLiteral("sensor5"));
        sensor5->setGeometry(QRect(430, 660, 111, 20));
        sensor5data = new QLabel(centralWidget);
        sensor5data->setObjectName(QStringLiteral("sensor5data"));
        sensor5data->setGeometry(QRect(450, 680, 81, 20));
        sensor6data = new QLabel(centralWidget);
        sensor6data->setObjectName(QStringLiteral("sensor6data"));
        sensor6data->setGeometry(QRect(451, 560, 81, 20));
        sensor6 = new QLabel(centralWidget);
        sensor6->setObjectName(QStringLiteral("sensor6"));
        sensor6->setGeometry(QRect(430, 540, 111, 19));
        speedLabel = new QLabel(centralWidget);
        speedLabel->setObjectName(QStringLiteral("speedLabel"));
        speedLabel->setGeometry(QRect(250, 470, 62, 19));
        speedPercent = new QLabel(centralWidget);
        speedPercent->setObjectName(QStringLiteral("speedPercent"));
        speedPercent->setGeometry(QRect(300, 470, 62, 19));
        sensorPlot1 = new QCustomPlot(centralWidget);
        sensorPlot1->setObjectName(QStringLiteral("sensorPlot1"));
        sensorPlot1->setGeometry(QRect(550, 30, 261, 101));
        sensorPlot1->setMinimumSize(QSize(120, 0));
        sensorPlot2 = new QCustomPlot(centralWidget);
        sensorPlot2->setObjectName(QStringLiteral("sensorPlot2"));
        sensorPlot2->setGeometry(QRect(550, 150, 261, 101));
        sensorPlot2->setMinimumSize(QSize(120, 0));
        sensorPlot3 = new QCustomPlot(centralWidget);
        sensorPlot3->setObjectName(QStringLiteral("sensorPlot3"));
        sensorPlot3->setGeometry(QRect(160, 630, 261, 101));
        sensorPlot3->setMinimumSize(QSize(120, 0));
        sensorPlot4 = new QCustomPlot(centralWidget);
        sensorPlot4->setObjectName(QStringLiteral("sensorPlot4"));
        sensorPlot4->setGeometry(QRect(550, 390, 261, 101));
        sensorPlot4->setMinimumSize(QSize(120, 0));
        sensorPlot5 = new QCustomPlot(centralWidget);
        sensorPlot5->setObjectName(QStringLiteral("sensorPlot5"));
        sensorPlot5->setGeometry(QRect(550, 630, 261, 101));
        sensorPlot5->setMinimumSize(QSize(120, 0));
        sensorPlot6 = new QCustomPlot(centralWidget);
        sensorPlot6->setObjectName(QStringLiteral("sensorPlot6"));
        sensorPlot6->setGeometry(QRect(550, 510, 261, 101));
        sensorPlot6->setMinimumSize(QSize(120, 0));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(70, 390, 91, 20));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(170, 390, 115, 32));
        fetchButton = new QPushButton(centralWidget);
        fetchButton->setObjectName(QStringLiteral("fetchButton"));
        fetchButton->setGeometry(QRect(300, 390, 115, 32));
        kpLabel = new QLabel(centralWidget);
        kpLabel->setObjectName(QStringLiteral("kpLabel"));
        kpLabel->setGeometry(QRect(10, 500, 21, 19));
        kdLabel = new QLabel(centralWidget);
        kdLabel->setObjectName(QStringLiteral("kdLabel"));
        kdLabel->setGeometry(QRect(10, 530, 21, 19));
        refLabel = new QLabel(centralWidget);
        refLabel->setObjectName(QStringLiteral("refLabel"));
        refLabel->setGeometry(QRect(10, 560, 31, 19));
        doubleSpinBox = new QDoubleSpinBox(centralWidget);
        doubleSpinBox->setObjectName(QStringLiteral("doubleSpinBox"));
        doubleSpinBox->setGeometry(QRect(40, 500, 71, 24));
        doubleSpinBox->setValue(7);
        doubleSpinBox_2 = new QDoubleSpinBox(centralWidget);
        doubleSpinBox_2->setObjectName(QStringLiteral("doubleSpinBox_2"));
        doubleSpinBox_2->setGeometry(QRect(40, 530, 71, 24));
        doubleSpinBox_2->setValue(26);
        spinBox = new QSpinBox(centralWidget);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(40, 560, 71, 24));
        spinBox->setValue(10);
        pdControlLabel = new QLabel(centralWidget);
        pdControlLabel->setObjectName(QStringLiteral("pdControlLabel"));
        pdControlLabel->setGeometry(QRect(10, 470, 151, 19));
        setParameterButton = new QPushButton(centralWidget);
        setParameterButton->setObjectName(QStringLiteral("setParameterButton"));
        setParameterButton->setGeometry(QRect(10, 590, 121, 31));
        sensorPlot0 = new QCustomPlot(centralWidget);
        sensorPlot0->setObjectName(QStringLiteral("sensorPlot0"));
        sensorPlot0->setGeometry(QRect(550, 270, 261, 101));
        sensorPlot0->setMinimumSize(QSize(120, 0));
        sensor0 = new QLabel(centralWidget);
        sensor0->setObjectName(QStringLiteral("sensor0"));
        sensor0->setGeometry(QRect(440, 290, 111, 20));
        sensor0data = new QLabel(centralWidget);
        sensor0data->setObjectName(QStringLiteral("sensor0data"));
        sensor0data->setGeometry(QRect(460, 310, 81, 20));
        temp90Button = new QPushButton(centralWidget);
        temp90Button->setObjectName(QStringLiteral("temp90Button"));
        temp90Button->setGeometry(QRect(170, 430, 115, 32));
        trimLeftLabel = new QLabel(centralWidget);
        trimLeftLabel->setObjectName(QStringLiteral("trimLeftLabel"));
        trimLeftLabel->setGeometry(QRect(190, 540, 111, 21));
        trimRightLabel = new QLabel(centralWidget);
        trimRightLabel->setObjectName(QStringLiteral("trimRightLabel"));
        trimRightLabel->setGeometry(QRect(190, 570, 111, 21));
        trimValueLeft = new QSpinBox(centralWidget);
        trimValueLeft->setObjectName(QStringLiteral("trimValueLeft"));
        trimValueLeft->setGeometry(QRect(310, 540, 49, 24));
        trimValueLeft->setMinimum(0);
        trimValueLeft->setMaximum(9);
        trimValueRight = new QSpinBox(centralWidget);
        trimValueRight->setObjectName(QStringLiteral("trimValueRight"));
        trimValueRight->setGeometry(QRect(310, 570, 49, 24));
        trimValueRight->setMinimum(0);
        trimValueRight->setMaximum(9);
        Gui->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Gui);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 871, 22));
        menuCommands = new QMenu(menuBar);
        menuCommands->setObjectName(QStringLiteral("menuCommands"));
        Gui->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Gui);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Gui->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Gui);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Gui->setStatusBar(statusBar);

        menuBar->addAction(menuCommands->menuAction());
        menuCommands->addAction(actionForward);
        menuCommands->addAction(actionDown);
        menuCommands->addAction(actionLeft);
        menuCommands->addAction(actionRight);
        menuCommands->addAction(actionSpeedUp);
        menuCommands->addAction(actionSlowDown);
        menuCommands->addAction(actionStop);
        menuCommands->addAction(actionSetParameter);
        menuCommands->addAction(actionHalt);

        retranslateUi(Gui);

        QMetaObject::connectSlotsByName(Gui);
    } // setupUi

    void retranslateUi(QMainWindow *Gui)
    {
        Gui->setWindowTitle(QApplication::translate("Gui", "ui", 0));
        actionForward->setText(QApplication::translate("Gui", "Forward", 0));
        actionForward->setShortcut(QApplication::translate("Gui", "Up", 0));
        actionDown->setText(QApplication::translate("Gui", "Backward", 0));
        actionDown->setShortcut(QApplication::translate("Gui", "Down", 0));
        actionLeft->setText(QApplication::translate("Gui", "Left", 0));
        actionLeft->setShortcut(QApplication::translate("Gui", "Left", 0));
        actionRight->setText(QApplication::translate("Gui", "Right", 0));
        actionRight->setShortcut(QApplication::translate("Gui", "Right", 0));
        actionSpeedUp->setText(QApplication::translate("Gui", "SpeedUp", 0));
        actionSpeedUp->setShortcut(QApplication::translate("Gui", "Alt+Up", 0));
        actionSlowDown->setText(QApplication::translate("Gui", "SlowDown", 0));
        actionSlowDown->setShortcut(QApplication::translate("Gui", "Alt+Down", 0));
        actionStop->setText(QApplication::translate("Gui", "Stop", 0));
        actionStop->setShortcut(QApplication::translate("Gui", "Space", 0));
        actionSetParameter->setText(QApplication::translate("Gui", "Set parameters", 0));
        actionSetParameter->setShortcut(QApplication::translate("Gui", "Ctrl+S", 0));
        actionHalt->setText(QApplication::translate("Gui", "Auto", 0));
        actionHalt->setShortcut(QApplication::translate("Gui", "Ctrl+X", 0));
        sensor1->setText(QApplication::translate("Gui", "Back Long", 0));
        sensor1data->setText(QApplication::translate("Gui", "sensor1data", 0));
        sensor2->setText(QApplication::translate("Gui", "Back short", 0));
        sensor2data->setText(QApplication::translate("Gui", "sensor2data", 0));
        sensor3->setText(QApplication::translate("Gui", "Right back short", 0));
        sensor3data->setText(QApplication::translate("Gui", "sensor3data", 0));
        sensor4->setText(QApplication::translate("Gui", "Front short", 0));
        sensor4data->setText(QApplication::translate("Gui", "sensor4data", 0));
        sensor5->setText(QApplication::translate("Gui", "Right front short", 0));
        sensor5data->setText(QApplication::translate("Gui", "sensor5data", 0));
        sensor6data->setText(QApplication::translate("Gui", "sensor6data", 0));
        sensor6->setText(QApplication::translate("Gui", "Left midrange", 0));
        speedLabel->setText(QApplication::translate("Gui", "Speed:", 0));
        speedPercent->setText(QApplication::translate("Gui", "50", 0));
        label->setText(QApplication::translate("Gui", "Connecting...", 0));
        pushButton->setText(QApplication::translate("Gui", "Connect", 0));
        fetchButton->setText(QApplication::translate("Gui", "Fetch", 0));
        kpLabel->setText(QApplication::translate("Gui", "Kp:", 0));
        kdLabel->setText(QApplication::translate("Gui", "Kd:", 0));
        refLabel->setText(QApplication::translate("Gui", "Ref:", 0));
        pdControlLabel->setText(QApplication::translate("Gui", "PD-control parameters:", 0));
        setParameterButton->setText(QApplication::translate("Gui", "Set parameters", 0));
        sensor0->setText(QApplication::translate("Gui", "Front long", 0));
        sensor0data->setText(QApplication::translate("Gui", "sensor0data", 0));
        temp90Button->setText(QApplication::translate("Gui", "90 turn done", 0));
        trimLeftLabel->setText(QApplication::translate("Gui", "Trim value left", 0));
        trimRightLabel->setText(QApplication::translate("Gui", "Trim value right", 0));
        menuCommands->setTitle(QApplication::translate("Gui", "Commands", 0));
    } // retranslateUi

};

namespace Ui {
    class Gui: public Ui_Gui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_H

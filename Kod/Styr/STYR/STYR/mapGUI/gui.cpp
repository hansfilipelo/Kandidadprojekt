#include "gui.h"
#include "ui_gui.h"

Gui::Gui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Gui)
{
    ui->setupUi(this);
    ui->speedPercent->setText(0);
    ui->mapView->
}

Gui::~Gui()
{
    delete ui;
}

void Gui::on_speedSlider_valueChanged(int value)
{
   QString integer = QString::number(value);
   ui->speedPercent->setText(integer);

   //Will send a command to the bluetooth to tell the robot to change speed
}

void Gui::setUpGrid() {

}

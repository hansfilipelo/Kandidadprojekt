#include "paramwindow.h"
#include "ui_paramwindow.h"

ParamWindow::ParamWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParamWindow)
{
    ui->setupUi(this);
}

ParamWindow::~ParamWindow()
{
    delete ui;
}

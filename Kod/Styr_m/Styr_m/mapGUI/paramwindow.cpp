#include "paramwindow.h"
#include "ui_paramwindow.h"

ParamWindow::ParamWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParamWindow)
{
    ui->setupUi(this);

    defKp = ui->kpSpinBox->value();
    defKd = ui->kdSpinBox->value();
    defRef = ui->refBox->value();
    defTrimLeft = ui->trimLeftBox->value();
    defTrimRight = ui->trimRightBox->value();
    defFwdRefLong = ui->fwdRefLongBox->value();
    defBwdRefLong = ui->bwdRefLongBox->value();
    defFwdRefShort = ui->fwdRefShortBox->value();
    defBwdRefShort = ui->bwdRefShortBox->value();
    defRightCornerFront = ui->rightCornerFrontBox->value();
    defRightCornerBack = ui->rightCornerBackBox->value();
    defRightWallFront = ui->rightWallFrontBox->value();
    defRightWallBack = ui->rightWallBackBox->value();
    defHaltAfterSection = (int)ui->haltBox->isChecked();

}

ParamWindow::~ParamWindow()
{
    delete ui;
}

void ParamWindow::setOrder(Order * inOrder)
{
    Bluetooth = inOrder;
}

void ParamWindow::on_closeButton_pressed()
{
    this->hide();
}

void ParamWindow::on_setParamButton_pressed()
{
    double Kp = ui->kpSpinBox->value();
    double Kd = ui->kdSpinBox->value();
    int Ref = ui->refBox->value();
    int trimLeft = ui->trimLeftBox->value();
    int trimRight = ui->trimRightBox->value();
    int fwdRefLong = ui->fwdRefLongBox->value();
    int bwdRefLong = ui->bwdRefLongBox->value();
    int fwdRefShort = ui->fwdRefShortBox->value();
    int bwdRefShort = ui->bwdRefShortBox->value();
    int rightCornerFront = ui->rightCornerFrontBox->value();
    int rightCornerBack = ui->rightCornerBackBox->value();
    int rightWallFront = ui->rightWallFrontBox->value();
    int rightWallBack = ui->rightWallBackBox->value();
    int haltAfterSection = (int)ui->haltBox->isChecked();

    Bluetooth->setControlParameters(Kp,Kd,Ref,trimLeft,trimRight,fwdRefLong,bwdRefLong,fwdRefShort,bwdRefShort,rightCornerFront,rightCornerBack,rightWallFront,rightWallBack,haltAfterSection);
}

void ParamWindow::on_setDefaultButton_pressed()
{
    Bluetooth->setControlParameters(defKp,defKd,defRef,defTrimLeft,defTrimRight,defFwdRefLong,defBwdRefLong,defFwdRefShort,defBwdRefShort,defRightCornerFront,defRightCornerBack,defRightWallFront,defRightWallBack,defHaltAfterSection);

}

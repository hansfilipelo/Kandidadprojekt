#ifndef PARAMWINDOW_H
#define PARAMWINDOW_H

#include <QDialog>

namespace Ui {
class ParamWindow;
}

class ParamWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ParamWindow(QWidget *parent = 0);
    ~ParamWindow();

private:
    Ui::ParamWindow *ui;
};

#endif // PARAMWINDOW_H

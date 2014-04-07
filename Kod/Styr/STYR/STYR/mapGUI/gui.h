#ifndef GUI_H
#define GUI_H

#include <QMainWindow>

namespace Ui {
class Gui;
}

class Gui : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gui(QWidget *parent = 0);
    ~Gui();
    void setUpGrid();

private slots:
    void on_speedSlider_valueChanged(int value);

private:
    Ui::Gui *ui;
};

#endif // UI_H

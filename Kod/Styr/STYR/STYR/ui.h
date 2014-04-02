#ifndef UI_H
#define UI_H

#include <QMainWindow>

namespace Ui {
class ui;
}

class ui : public QMainWindow
{
    Q_OBJECT

public:
    explicit ui(QWidget *parent = 0);
    ~ui();

private:
    Ui::ui *ui;
};

#endif // UI_H

#include "gui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Gui w;
    w.show();
    int connected = 0;
    for(int i=0; i < 5; i++){
        if(w.startPort() == 1){
            continue;
        }

     connected = 1;
     w.labelSet("Connected");
     break;
    }
    w.connectStatus = connected;
   // w.giveValues(5);
    return a.exec();
}

#include "Communication.h"
#include "../../../sensormodul/sensormodul/slave.h

Communication::Communication(Slave* pointer){
    slavePointer=pointer1;
    robotPointer=pointer2;
}

void Communication::handleData(){
    for (int i=0;i<25;i++){
        inData[i]=slavePointer->inDataArray[i];
    }
    dataLength = (int)inData[0];
    if (inData[1]=='r' && inData[2]==0) {
        robotPointer->rotateLeft();
    }
    if (inData[1]=='r' && inData[2]==1) {
        robotPointer->rotateRight();
    }
    if (inData[1]=='f'){
        int speed =(int)inData[3];
        robotPointer->driveForward(speed);
    }
    if (inData[1]=='b'){
        int speed =(int)inData[3];
        robotPointer->driveBackward(speed);
    }
    
}
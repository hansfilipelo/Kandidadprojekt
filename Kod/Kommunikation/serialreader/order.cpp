//
//  commander.cpp
//  
//
//  Created by Jens Edhammer on 2014-04-08.
//
//
#include "order.h"

Order::Order(SerialPort *inPort){
    serport = inPort;
}

Order::~Order(){
}

void Order::rotateLeft(){

    char outData[25] = {0};
    outData[0] = 2;
    outData[1] = 'r';
    outData[2] = 0;
    serport->sendArray(outData);
}


void Order::rotateRight(){
    char outData[25] = {0};
    outData[0] = 2;
    outData[1] = 'r';
    outData[2] = 1;
    serport->sendArray(outData);
}


void Order::halt(){
    char outData[25] = {0};
    outData[0] = 1;
    outData[1] = 'h';
    serport->sendArray(outData);
}


void Order::forward(){
    char outData[25] = {0};
    outData[0] = 1;
    outData[1] = 'f';
    serport->sendArray(outData);
}


void Order::backward(){
    char outData[25] = {0};
    outData[0] = 1;
    outData[1] = 'b';
    serport->sendArray(outData);
}

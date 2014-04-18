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

/*Constructs the array that are to be sent to AVR processor.
 */

void Order::setPID(int kp, int kd){
    char data[27] = {4,'P',0,(char)kp,(char)kd,'Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    serport->sendArray(data);
}

void Order::rotateLeft(int speed){

    char data[27] = {3,'r',0,(char)speed,'Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    serport->sendArray(data);
}


void Order::rotateRight(int speed){

    char data[27] = {3,'r',1,(char)speed,'Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    serport->sendArray(data);
}


void Order::halt(){
    
    char data[27] = {1,'h','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    serport->sendArray(data);
}


void Order::forward(int speed){

    char data[27] = {3,'f','Z',(char)speed,'Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    serport->sendArray(data);
}


void Order::backward(int speed){

    char data[27] = {3,'b','Z',(char)speed,'Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    serport->sendArray(data);
}

void Order::test(){
    
    char data[27] = {4,'B','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    serport->sendArray(data);
}

void Order::fetchMap(){

    char data[27] = {1,'F','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    serport->sendArray(data);
}

void Order::setControlParameters(double Kp, double Kd, int Ref){

}

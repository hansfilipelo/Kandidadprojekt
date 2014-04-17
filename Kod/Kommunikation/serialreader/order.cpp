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

void Order::rotateLeft(int inSpeed){

    char speed = inSpeed;
    char data[25] = {3,'r',0,speed,'Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    serport->sendArray(data);
}


void Order::rotateRight(int inSpeed){

    char speed = inSpeed;
    char data[25] = {3,'r',1,speed,'Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    serport->sendArray(data);
}


void Order::halt(){
    
    char data[25] = {1,'h','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    serport->sendArray(data);
}


void Order::forward(int inSpeed){

    char speed = inSpeed;
    char data[25] = {3,'f','Z',speed,'Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    serport->sendArray(data);
}


void Order::backward(int inSpeed){

    char speed = inSpeed;
    char data[25] = {3,'b','Z',speed,'Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    serport->sendArray(data);
}

void Order::test(){
    
    char data[25] = {4,'B',3,2,0,'Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    serport->sendArray(data);
}

void Order::fetchMap(){

    char data[25] = {1,'F','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    serport->sendArray(data);
}

void Order::setControlParameters(double Kp, double Kd, int Ref){

}

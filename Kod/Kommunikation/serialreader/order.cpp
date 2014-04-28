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

//void Order::setPID(int kp, int kd){
//    char data[27] = {4,'P',0,(char)kp,(char)kd,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
//    serport->sendArray(data);
//}

void Order::rotateLeft(int speed){

    char data[27] = {3,'r',0,(char)speed,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
}


void Order::rotateRight(int speed){

    char data[27] = {3,'r',1,(char)speed,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
}


void Order::halt(){
    
    char data[27] = {1,'h',30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
}


void Order::forward(int speed){

    char data[27] = {3,'f',30,(char)speed,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
}


void Order::backward(int speed){

    char data[27] = {3,'b',30,(char)speed,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
}

void Order::test(){
    
    char data[27] = {4,'B',30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
}

void Order::fetchMap(){

    char data[27] = {1,'F',30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
}

void Order::setControlParameters(double Kp, double Kd, int Ref){
    //Separates the parts of the doubles and sends them as chars.
    int Kp1=floor(Kp);
    int Kp01=floor(10*Kp);
    int Kp001=floor(100*Kp);

    int Kd1=floor(Kd);
    int Kd01=floor(10*Kd);
    int Kd001=floor(100*Kd);

    std::cout << kp1 << kp01 << kp001 << std::endl;

    char data[27] = {9,'P',0,(char)Kp1,(char)Kp01,(char)Kp001,(char)Kd1,(char)Kd01,(char)Kd001,(char)Ref,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
}

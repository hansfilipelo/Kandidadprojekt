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

    char data[25] = {2,'r',0,'Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    
    serport->sendArray(data);
}


void Order::rotateRight(){
    char data[25] = {2,'r',1,'Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    
    serport->sendArray(data);
}


void Order::halt(){
    
    char data[25] = {1,'h','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    std::cout<<"halt" << std::endl;
    serport->sendArray(data);
}


void Order::forward(){
    char data[25] = {1,'f','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    
    serport->sendArray(data);
}


void Order::backward(){
    
    char data[25] = {1,'b','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    
    serport->sendArray(data);
}

void Order::test(){
    
    char data[25] = {4,'B',3,2,0,'Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z','Z'};
    
    serport->sendArray(data);
}


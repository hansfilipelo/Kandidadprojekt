//
//  order.h
//  
//
//  Created by Jens Edhammer on 2014-04-08.
//
//

class SerialPort;

#ifndef ____order__
#define ____order__

#include <iostream>
#include "serialport.h"

class Order{

public:
    
    Order(SerialPort*);
    ~Order();

   // void setPID(int,int);
    void rotateLeft(int);
    void rotateRight(int);
    void turnDone();
    void halt(int);
    void forward(int);
    void backward(int);
    void test();
    void fetchMap();
    void setControlParameters(double, double, int);
    
    int getTen(double); //Returns ex. 1 if the number is 10
    int getOne(double); //Returns ex 1 if the number is 1
    int getTenth(double); //Returns ex. 1 if the number is 0.1
    int getHundreth(double); //Returns ex. 1 if the number is 0.01
    
protected:
    
private:
    
    SerialPort* serport;
    
    
};

#endif /* defined(____order__) */

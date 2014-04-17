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

    void setPID(int,int);
    void rotateLeft(int);
    void rotateRight(int);
    void halt();
    void forward(int);
    void backward(int);
    void test();
    void fetchMap();
    void setControlParameters(double, double, int);
    
protected:
    
private:
    
    SerialPort* serport;
    
    
};

#endif /* defined(____order__) */

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

    void rotateLeft();
    void rotateRight();
    void halt();
    void forward();
    void backward();
    void test();
    
protected:
    
private:
    
    SerialPort* serport;
    
    
};

#endif /* defined(____order__) */

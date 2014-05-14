/*
 *  MapMaster2001
 *  Erik Ekelund, David Habrman, Tobias Grundström,
 *  Hans-Filip Elo, Niklas Ericsson, Jens Edhammer
 *
 *  TSEA56 2014.
 *
 */


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
    void rowReceived();
    void rotateLeft(int);
    void rotateRight(int);
    void turnDone();
    void halt();
    void forward(int);
    void backward(int);
    void test();
    void fetchMap();
    void autonom(int);
    void setControlParameters(double,double,int,int,int,int,int,int,int,int,int,int,int,int,int);
    
    int getTen(double); //Returns ex. 1 if the number is 10
    int getOne(double); //Returns ex 1 if the number is 1
    int getTenth(double); //Returns ex. 1 if the number is 0.1
    int getHundreth(double); //Returns ex. 1 if the number is 0.01
    
protected:
    
private:
    
    SerialPort* serport;
    
    
};

#endif /* defined(____order__) */

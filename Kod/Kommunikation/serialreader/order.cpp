/******************************************************
*
*Code was produced as part of the project MapMaster2001
*
*File: order.cpp
*Purpose: Class for all commands sent from PC to robot.
*
*Created by: Jens Edhammer (jened502), Tobias Grundström (tobgr602) 
*
********************************************************/
#include "order.h"

Order::Order(SerialPort *inPort){
    serport = inPort;
}

Order::~Order(){
}


void Order::btReset(){

    char data[27] = {'x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'};
    serport->sendArray(data);
}

void Order::rotateLeft(int speed){

    char data[27] = {3,'r',0,(char)speed,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
}


void Order::rotateRight(int speed){

    char data[27] = {3,'r',1,(char)speed,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
}

void Order::turnDone()
{
    char data[27] = {1,'G',30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
    std::cout << "Done" << std::endl;
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

void Order::autonom(int speed){

    char data[27] = {3,'a',30,(char)speed,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
}

/*
 *	SetControlParameters. Converst the doubles and ints to char to be sent over bluetooth.
 */

void Order::setControlParameters(double Kp, double Kd, int Ref,int trimLeft,int trimRight){
    
    char data[27] = {13,'P',0,(char)getTen(Kp),(char)getOne(Kp),(char)getTenth(Kp),(char)getHundreth(Kp),(char)getTen(Kd),(char)getOne(Kd),(char)getTenth(Kd),(char)getHundreth(Kd),(char)Ref,(char)trimLeft,char(trimRight),30,30,30,30,30,30,30,30,30,30,30,30,30};
    serport->sendArray(data);
}

/*
 *	Gives the tens of a double. for example 13.45 would return 1. 
 */


int Order::getTen(double number){
    return ((int)floor(number/10) %10);
}


/*
 *	Gives the ones of a double. for example 13.45 would return 3. 
 */


int Order::getOne(double number){
    return ((int)floor(number) %10);
}


/*
 *	Gives the tenths of a double. for example 13.45 would return 4. 
 */

int Order::getTenth(double number){
    return ((int)floor(number*10) %10);
}


/*
 *	Gives the hundreths of a double. for example 13.45 would return 5. 
 */
int Order::getHundreth(double number){
    return ((int)floor(number*100) %10);
}




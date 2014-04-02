//
//  Robot.h
//  
//
//  Created by Tobias Grundström on 2014-04-02.
//
//
#include <string>
#include <>

#ifndef _Robot_h
#define _Robot_h

class Robot : MapSection
{
public:
    Robot(int xPos, int yPos);
    
    void changeDirection(string direction);
    void drive(int speed)
    void moveTo(int xPos, int yPos);
    void sensorValuesIn(double fwd, double bwd, double left, double leftLong, double right, double phiDot)
    
    
private:
    vector<double> fwdSensor;
    vector<double> bwdSensor;
    vector<double> leftSensor;
    vector<double> rightSensor;
    vector<double> phiDot;
    
};


#endif

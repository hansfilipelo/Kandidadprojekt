/*
 * lcd.h
 *
 * Created: 4/14/2014 11:13:42 AM
 *  Author: davha227
 */



#ifndef BLUETOOTH
#define BLUETOOTH


#include <avr/io.h>
#include <util/delay.h>
#define LCD_data PORTC //ska denna vara kvar? Automerge gjorde massa skit.


class Lcd
{
public:
    Lcd();
    char shift = 0x00;
    void setKp(int value);
    void setKd(int value);
    void updateSensor(int sensornid, int value);
    void SetData(unsigned char var);
    void init();
    void command(unsigned char var);
    void senddata(unsigned char var);
    void draw(unsigned char location, unsigned char sign);
    void reset();
    void drawSensorNames();
				
};
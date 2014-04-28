/*
 * lcd.h
 *
 * Created: 4/14/2014 11:13:42 AM
 *  Author: davha227
 */



#ifndef LCD
#define LCD

#include <avr/io.h>
#define F_CPU 14.7456E6
#include <util/delay.h>

class Lcd
{
public:
    Lcd();
    char shift = 0x00;

    void SetData(unsigned char);
    void init();
    void command(unsigned char);
    void senddata(unsigned char);
    void draw(unsigned char, unsigned char);
    void reset();
    void drawSensorNames();
    void firstDraw(unsigned char,unsigned char);
    void insertSensorValuesToBuffer(unsigned char*);
	
	void update();

private:
    bool ready();
	bool moveToggle = true;
    
    int getCol(int);
    int getRow(int);
    bool drawSucceded = true;
    
	unsigned int writeValue = 0;
	unsigned char writePosition = 0;
    unsigned int writeBuffer[4][16];
    unsigned int sensorCounter = 0;
    unsigned int internalCounter = 0;
};

#endif
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
    void insertSensorValuesToBuffer(int, char, char, char);
	
	void update();
	void updateS1(char,char,char);
	void updateS2(char,char,char);
	void updateS3(char,char,char);
	void updateS4(char,char,char);
	void updateL1(char,char,char);
	void updateL2(char,char,char);
	void updateM1(char,char,char);
	void updateKP(int);
	void updateKD(int);



private:
    bool ready();
	bool moveToggle = true;
    
    int getCol(int);
    int getRow(int);
    bool drawSucceded;
    
    
    unsigned int writeBuffer[16][4];
    unsigned int sensorCounter = 0;
    
};

#endif
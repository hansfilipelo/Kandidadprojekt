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

    void SetData(unsigned char var);
    void init();
    void command(unsigned char var);
    void senddata(unsigned char var);
    void draw(unsigned char location, unsigned char sign);
    void reset();
    void drawSensorNames();
	
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
	char* getBuffer(int);
	bool ready();
	bool moveToggle = true;
	
	int writeBuffer[16][4];
	
	
	char bufferS1[5];
	char bufferS2[5];
	char bufferS3[5];
	char bufferS4[5];
	char bufferL1[5];
	char bufferL2[5];
	char bufferM1[5];
	
	int bufferStatus = 10;
};

#endif
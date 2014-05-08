


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
    void SetData(unsigned char);
    void init();
    void command(unsigned char);
    void senddata(unsigned char);
    void draw(unsigned char, unsigned char);
    void reset();
    void drawSensorNames();
    void firstDraw(unsigned char, unsigned char);
    void insertSensorValuesToBuffer(unsigned char*);
	void update();

	char shift = 0x00;
	bool bufferWritten = false;
	unsigned int RfidOnes = 0;
	unsigned int RfidTens = 0;

private:
	bool ready();
	unsigned int getNewPosition(int, int);
    int getCol(int);
    int getRow(int);
	
	unsigned int sensorCounter = 0;
	bool moveToggle = true;
	unsigned int position = 0;
	int row = 0;
	int col = 0;
    bool drawSucceded = true;
	int internalCounter = 0;
	unsigned char writeBuffer[4][16]={{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
									{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
									{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
									{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}};
};

#endif
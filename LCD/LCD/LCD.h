#include <avr/io.h>
#include <util/delay.h>
#define LCD_data PORTC //ska denna vara kvar? Automerge gjorde massa skit.


class Lcd
{
	public:
		char shift = 0x00;
		void setKp(int value);
		void setKd(int value);
		void updateSensor(int sensornid, int value);
		void SetData(unsigned char var);
		void LCD_init();
		void LCD_command(unsigned char var);
		void LCD_senddata(unsigned char var);
		void LCD_draw(unsigned char location, unsigned char sign);		
		void reset();
		Lcd();				
	};
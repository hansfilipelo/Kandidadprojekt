/*
 * BlueTooth2.cpp
 *
 * Created: 4/10/2014 4:31:31 PM
 *  Author: jened502
 */ 

#include <avr/io.h>
#define F_CPU 14.7456E6
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char data;
 // Data arrives in order, datalength (data.length + codeword + arg), codeword, arg, data. 
unsigned char inDataArray[25];
unsigned char outDataArray[25];
unsigned char pcInDataArray[25] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
unsigned char pcOutDataArray[25] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
unsigned int  position = 0;
unsigned char pcHandle[25];

bool Btrec = false; 
/*
*	BLUETOOTH
*/

void USART_Init( unsigned int baud )
{
	
	DDRD |= (1<<PORTD4)|(1<<PORTD3);
	PORTD &= ~((1<<PORTD4)|(1<<PORTD3));
	/* Set baud rate */
	UBRR0H = (unsigned char)(baud>>8);
	UBRR0L = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive(void)
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	unsigned char readData = UDR0;

	return readData;
}

void BluetoothSendArray(){
	
	for (unsigned int i=0; i<25; i++){
		USART_Transmit(pcOutDataArray[i]);
	}
}


volatile void handleBluetooth(){
	
	pcOutDataArray[0] = 7;
	pcOutDataArray[1] = 'D';
	
	pcOutDataArray[24] = '1';
	
	
	if(pcInDataArray[1] == 'B'){
		BluetoothSendArray(); //send data to module 0 (sensors)
	}
}




 void BluetoothReceive(){
	
	//Set CTS and RTS to 1
	PORTD |= (1<<PORTD4)|(1<<PORTD3);
	pcInDataArray[position] = USART_Receive();
	position++;
	if(position == 25){
		Btrec = true;
		position = 0;
	}
	PORTD &= ~((1<<PORTD4)|(1<<PORTD3));
}

/*
*	INTERUPTS
*/
//bluetooth interupt
ISR(USART0_RX_vect)
{	
	BluetoothReceive();	
}

int Calc(unsigned int inData){
	volatile int out = 1;
	for(volatile unsigned int i=2; i < inData; i++){
		out = out * i;
		asm("");
	}
	return out; 
}


int main(void)
{
	USART_Init(7);
	sei();
	volatile double calculatedData = 0;
	while(!Btrec){
		asm("");
		calculatedData = Calc(6);
		}
	
	handleBluetooth();
	
	while(1){
		asm("");
		calculatedData = Calc(5);
		
	}
	
}
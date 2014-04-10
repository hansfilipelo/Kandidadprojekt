/*
 * master.cpp
 *
 * Created: 4/7/2014 9:13:57 AM
 *  Author: jened502
 */ 


#include <avr/io.h>
#define F_CPU 14.7456E6
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

unsigned char data;
 // Data arrives in order, datalength (data.length + codeword + arg), codeword, arg, data. 
unsigned char inDataArray[25];
unsigned char outDataArray[25];
unsigned char pcInDataArray[25] = {0};
unsigned char pcOutDataArray[25] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
unsigned int  pcPosition = 0;
unsigned char pcHandle[25];

bool Btrec = false; 


/*
*	SPI-BUS 
*/

void SPI_MasterInit(void)
{
	//Enable SPI, sets it low
	PRR0 &= ~(1<<PRSPI);
	//Sets slave select
	DDRA = (1<<DDA0)|(1<<DDA1); 
	/* Set MOSI and SCK output, all others input */
	DDRB = (1<<DDB5)|(1<<DDB7)|(1<<DDB4);
	PORTA |= (1<<PORTA0);
	PORTA |= (1<<PORTA1);
	PORTB |= (1<<PORTB4);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	//Enable interupts
	EIMSK |= (1<<INT0)|(1<<INT2)|(1<<INT1);
	EICRA |= (1<<ISC01)|(1<<ISC00)|(1<<ISC11)|(1<<ISC10)|(1<<ISC21)|(1<<ISC20);
	
	
}

char SPI_MasterTransmit(char outData, unsigned int slave)
{
	char inData;
	//choose slave
	if (slave == 1){
		PORTA &= ~(1<<PORTA0);
	}
	else{
		PORTA &=~(1<<PORTA1);
	}
	/* Start transmission */
	SPDR = outData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	//store possible indata
	inData = SPDR;
	//Synchronize the slave by pulling SS high
	if (slave == 1){
		PORTA |= (1<<PORTA0);
	}
	else{
		PORTA |= (1<<PORTA1);
	}
	return inData;
}



void SPISendArray(unsigned int slave){	
	
	unsigned int length = 0;
	length = outDataArray[0];
	for (unsigned int i=0; i<=length; i++)
	{
		SPI_MasterTransmit(outDataArray[i], slave);
	}
}



void SPIReceiveArray(unsigned int slave){
	
	inDataArray[0] = SPI_MasterTransmit(0x00,slave);
	unsigned int length = inDataArray[0];
	for (unsigned int i=1; i<=length; i++)
	{
		inDataArray[i] = SPI_MasterTransmit(0x00, slave);
	}
}


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
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
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
	
	for (unsigned int i=0; i<=25; i++){
		USART_Transmit(pcOutDataArray[i]);
	}
}


void handleBluetooth(){
	if(!Btrec){
		return;
	}
	
	memcpy(pcHandle,pcInDataArray,25);
	pcOutDataArray[0] = 2;
	pcOutDataArray[1] = 'r';
	
	
	if(pcInDataArray[1] == 'r'){
		SPISendArray(1);
	}
	else{
		SPISendArray(1); //send data to module 1 (control)
	}
}


void BluetoothReceiveArray(){
	
	pcInDataArray[pcPosition] = USART_Receive();
	pcPosition++;
	if(pcPosition == 25){
		Btrec = true;
		pcPosition = 0;
		handleBluetooth();
	}
	PORTD &= ~((1<<PORTD4)|(1<<PORTD3));
}

/*
*	INTERUPTS
*/
//bluetooth interupt
ISR(USART0_RX_vect)
{	
	//Set CTS and RTS to 1
	PORTD |= (1<<PORTD4)|(1<<PORTD3);
	BluetoothReceiveArray();	
}


//Control module wants to send data
ISR(INT2_vect){
	cli();
	SPIReceiveArray(1);
	sei();
}
//Sensor module wants to send data
ISR(INT1_vect){
	cli();
	SPIReceiveArray(0);
	sei();
}

ISR(INT0_vect){
	cli();
	outDataArray[0] = 2;
	outDataArray[1] = 3;
	outDataArray[2] = 4;
	SPISendArray(1);
	sei();
}


int main(void)
{
	SPI_MasterInit();
	USART_Init(7);
	sei();
	/*
	while (!Btrec){
		int dummy = 0;
		dummy = dummy + 2;
		dummy = dummy + 3;
		dummy = dummy + 4;
		SPI_MasterTransmit('D',1);
	}
	
	*/
	while(1){
		int fummy = 0;
		fummy = fummy + 2; 
	}
	
}
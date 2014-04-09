/*
 * BlueTooth1.cpp
 *
 * Created: 3/27/2014 11:37:06 AM
 *  Author: jened502
 */ 

#include <avr/io.h>
#define F_CPU 14.7456E6
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char data;
unsigned int pcPos;
unsigned char inDataArray[25]; // Data arrives in order, datalength (data.length + codeword + type), codeword, type, data. 
unsigned char outDataArray[25];
unsigned char pcDataArray[25];
unsigned char pcInDataArray[25];
bool dataRec = false;



void SPI_MasterInit(void)
{
	//Enable SPI, sets it low
	//PRR0 &= ~(1<<PRSPI);
	//Sets slave select
	DDRA = (1<<DDA0)|(1<<DDA1); 
	/* Set MOSI and SCK output, all others input */
	DDRB = (1<<DDB5)|(1<<DDB7)|(1<<DDB4);
	PORTA |= (1<<PORTA0);
	PORTA |= (1<<PORTA1);
	PORTB |= (1<<PORTB4);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
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



void SPISendArray(unsigned int slave)
{	
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

void USART_Init( unsigned int baud )
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(baud>>8);
	UBRR0L = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
}

ISR(USART0_RX_vect)
{
	
	PORTD |= (1<<PORTD4)|(1<<PORTD3);
	pcInDataArray[pcPos++] = USART_Receive();
	PORTD &= ~((1<<PORTD4)|(1<<PORTD3));
	
}

int main(void)
{
	/*
	//Interrupts
	cli();
    //DDRDn = 1 for output
	DDRD = 0x1A;
	PIND = 0x02;
	USART_Init(7);
	sei(); 
	*/
	
	//enable interupt
	EIMSK |= (1<<INT0)|(1<<INT2);
	EICRA |= (1<<ISC01)|(1<<ISC00)|(1<<ISC21)|(1<<ISC20);
	
	SPI_MasterInit();
	USART_Init(7);
	sei();
	while(1){
		if(dataRec){
			if(pcInDataArray[1] == 't'){
				pcDataArray[0] = 7;
				pcDataArray[0] = 'A';
				pcDataArray[0] = 'B';
				pcDataArray[0] = 'C';
				pcDataArray[0] = 'D';
				pcDataArray[0] = 'E';
				pcDataArray[0] = 'F';
				pcDataArray[0] = 'G';
				
				
			}
		}
		
	}
	
	

}

ISR(INT0_vect){
	cli();
	outDataArray[0] = 0x02;
	outDataArray[1] = 'A';
	outDataArray[2] = 'A';
	SPISendArray(1);
	sei();
}

ISR(INT2_vect){
	cli();
	SPIReceiveArray(1);
	sei();
}
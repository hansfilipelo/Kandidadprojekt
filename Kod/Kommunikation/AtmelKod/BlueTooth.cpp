/*
 * BlueTooth1.cpp
 *
 * Created: 3/27/2014 11:37:06 AM
 *  Author: jened502
 */


#include <avr/io.h>
//#include <util/delay.h>
#include <avr/interrupt.h>

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
        ;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
        ;
	/* Get and return received data from buffer */
	return UDR0;
}

void USART_Init( unsigned int baud )
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(baud>>8);
	UBRR0L = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
}

ISR(USART0_RX_vect){
	
	unsigned char data1;
	data1 = USART_Receive();
    
	USART_Transmit('[');
	USART_Transmit(data1);
	USART_Transmit(']');
	
}

int main(void)
{
	
	//Interrupts
	cli();
    //DDRDn = 1 for output
	DDRD = 0x1A;
	PIND = 0x02;
	USART_Init(7);
	sei();
	
	while(1);
	
	
	
	return 0; 
}


#include <avr/io.h>
#include <string.h>
#include "uart.h"

void init_Uart()
{
	// Baud rate 4800
	UBRR0H = 0;
	UBRR0L = 12;
	
	// Enable receiver and transmitter
	//UCSR0B = ( 1 << RXEN0) | ( 1 << TXEN0);
	
	// Enable receiver and transmitter, Rx complete interrupt enable
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
	
	// Set frame format
	UCSR0C=(0 << USBS0) | ( 3 << UCSZ00 );
}

void TxData(char data)
{
	while (!(UCSR0A & (1<<UDRE0)));
	// Put data in to buffer
	UDR0=data;
}

void UART_Transmit_String(char s[])
{
	char cIndex = 0;
	while (s[cIndex]!=0)
	{
		TxData(s[cIndex]);
		cIndex++;
	}
}

int strlen2(char s[])
{
	int i = 0;
	while(s[i] != 0)
		i++;
	return i;
}

void reverse(char s[])
{
	int i, j;
	char c;

	for( i = 0, j = strlen2(s)-1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

void UART_itoa(int n, char s[])
{
	int i, sign;

	if((sign = n) < 0) // record sign
		n = -n; // make n positive
	i = 0;
	do { // generate digits in reverse order
		s[i++] = n % 10 + '0'; // get next digit
	} while((n /= 10) > 0); // delete it
	if(sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse(s);
}

void UART_Transmit_Decimal(int i)
{
	char string[16];

	UART_itoa(i, string);
	UART_Transmit_String(string);
	TxData(',');
}
#include <avr/io.h>

void USART_Init(unsigned int ubrr) {
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)(ubrr);
	UCSRB = (1 << RXEN)|(1 << TXEN);
	UCSRC = (1 << UCSZ0)|(1 << UCSZ1);
}

void USART_Transmit_char (unsigned char data) {
	while(!(UCSRA & (1 << UDRE)));
	UDR = data;
}

void USART_putstring(char* StringPtr) {
	while(*StringPtr != 0x00){    //Here we check if there is still more chars to send, this is done checking the actual char and see if it is different from the null char
		USART_Transmit_char(*StringPtr);    //Using the simple send function we send one char at a time
	StringPtr++;}        //We increment the pointer so we can read the next char
}

void USART_Receive(void) {
	while(!(UCSRA & (1 << RXC)));
	return UDR;
}

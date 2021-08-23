#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"
#define _NOP() do { __asm__ __volatile__ ("nop"); } while (0)
#define OUTP PB4
#define INP PB3










#define C_MinBaudRate_U32 2400
#define C_MaxBaudRate_U32 2500000UL
#define util_ExtractByte0to8(x)    (uint8_t) ((x) & 0xFFu)
#define util_ExtractByte8to16(x)   (uint8_t) (((x)>>8) & 0xFFu)
#define M_GetBaudRateGeneratorValue(baudrate)  (((F_CPU -((baudrate) * 8L)) / ((baudrate) * 16UL)))
#define  util_GetBitMask(bit)          (1<<(bit))
#define  util_IsBitCleared(x,bit)      (((x)&(util_GetBitMask(bit)))==0u)
void UART_SetBaudRate(uint32_t v_baudRate_u32)
{
	uint16_t RegValue;

		RegValue = M_GetBaudRateGeneratorValue(v_baudRate_u32);
	UBRRL = util_ExtractByte0to8(RegValue);
	UBRRH = util_ExtractByte8to16(RegValue);
}

void UART_Init(uint32_t v_baudRate_u32)
{
	UCSRB= (1<<TXEN);                  // Enable Receiver and Transmitter
	UCSRC= (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);   // Async-mode
	UCSRA= 0x00;                 // Clear the UASRT status register
	/* UART_SetBaudRate(v_baudRate_u32); */
}
void UART_TxChar(char v_uartData_u8)
{
	while(util_IsBitCleared(UCSRA,UDRE)); // Wait till Transmitter(UDR) register becomes Empty
	UDR =v_uartData_u8;                              // Load the data to be transmitted
}











volatile uint16_t time;
uint16_t map[] = {
	100,
	100,
	100,
	100,
	100, //dasghjh
	100,
	100,
	100
};                

int main(void){
    USART_Init(19200);
    while(1){
        USART_putstring("dupa");
        _delay_ms(1000);
    }
	/* DDRB = _BV(OUTP); */

	/* PORTB = _BV(PB3) | _BV(PB4); */
	/* while(1){ */
	/* 	PORTB = 0; */
	/* 	_delay_ms(500); */
	/* 	PORTB = _BV(OUTP); */
	/* 	_delay_ms(500); */
	/* }; */
	/* //pins init */
	/* DDRB = _BV(OUTP); */
	/* PORTB = _BV(INP); */
	/* //pin interrupt */
	/* /1* MCUCR |= (1<<ISC00) | (1<<ISC01); //INT0 as rising edg *1/ */
	/* /1* PCMSK |= (1<<PCINT3);   // pin change mask: listen to portb, pin PB3 *1/ */
    	/* /1* GIMSK |= (1<<PCIE); // enable PCINT interrupt *1/ */
	/* //timer init */
	/* TCCR0A |= _BV(WGM01); //CTC */
	/* TCCR0B |= _BV(CS01)|_BV(CS00); //prescaler 64 */
	/* OCR0A = 18; //cycle 19 */
	/* TIMSK0 |= _BV(OCIE0A); //enable timer intrr */
	/* sei(); */
	/* while(1){ */
	/* 	if(~(PINB&_BV(INP))){ */
	/* 		pickup(); */
	/* 	} */
	/* } */
}
//pickup
void pickup(){
	uint16_t t = time;
	time = 0;
	uint8_t index = 1232435/t;
	if(index > 32)
		index=32;
	_delay_us(100);
	PORTB |=_BV(OUTP);
	_NOP();
	_NOP();
	_NOP();
	PORTB ^=_BV(OUTP);
	
}
//timer
ISR(TIM0_COMPA_vect)
{
	time++;
}








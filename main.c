#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define _NOP() do { __asm__ __volatile__ ("nop"); } while (0)
#define UART_TX PB1
#define UART_RX PB0
#define OUTP PB4
#define INP PB3

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
	DDRB = _BV(OUTP);
	PORTB = _BV(PB3) | _BV(PB4);
	while(1){
		PORTB = 0;
		_delay_ms(500);
		PORTB = _BV(OUTP);
		_delay_ms(500);
	};
	//pins init
	DDRB = _BV(OUTP);
	PORTB = _BV(INP);
	//pin interrupt
	/* MCUCR |= (1<<ISC00) | (1<<ISC01); //INT0 as rising edg */
	/* PCMSK |= (1<<PCINT3);   // pin change mask: listen to portb, pin PB3 */
    	/* GIMSK |= (1<<PCIE); // enable PCINT interrupt */
	//timer init
	TCCR0A |= _BV(WGM01); //CTC
	TCCR0B |= _BV(CS01)|_BV(CS00); //prescaler 64
	OCR0A = 18; //cycle 19
	TIMSK0 |= _BV(OCIE0A); //enable timer intrr
	sei();
	while(1){
		if(~(PINB&_BV(INP))){
			pickup();
		}
	}
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

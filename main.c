#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define _NOP() do { __asm__ __volatile__ ("nop"); } while (0)
#define OUTP PB0
#define INP PD2

#define C_MinBaudRate_U32 2400
#define C_MaxBaudRate_U32 2500000UL
#define util_ExtractByte0to8(x)    (uint8_t) ((x) & 0xFFu)
#define util_ExtractByte8to16(x)   (uint8_t) (((x)>>8) & 0xFFu)
#define M_GetBaudRateGeneratorValue(baudrate)  (((F_CPU -((baudrate) * 8L)) / ((baudrate) * 16UL)))
#define  util_GetBitMask(bit)          (1<<(bit))
#define  util_IsBitCleared(x,bit)      (((x)&(util_GetBitMask(bit)))==0u)

#define true 1
#define false 0

void UART_SetBaudRate(uint32_t v_baudRate_u32)
{
	uint16_t RegValue;

	RegValue = M_GetBaudRateGeneratorValue(v_baudRate_u32);
	UBRRL = util_ExtractByte0to8(RegValue);
	UBRRH = util_ExtractByte8to16(RegValue);
}

void UART_Init()
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

volatile uint8_t timeout = true;

/* uint16_t map[] = { */
/* 	100, */
/* 	100, */
/* 	100, */
/* 	100, */
/* 	100, //dasghjh */
/* 	100, */
/* 	100, */
/* 	100 */
/* }; */                

uint16_t cumT = 0;
uint16_t lastchr = 0;
volatile uint16_t t0;

void spark(){
    PORTB |= _BV(OUTP);
    _delay_us(500);
    PORTB ^= _BV(OUTP);
}

void wait(t){
    while(TCNT1<t){
    }
    return;
}

uint8_t checkImp(uint16_t chr) {
    if(chr > 4000){
        return true;
    }
    if(chr >= lastchr/2){
        return true;
    }
    return false;
}

void setTimer(uint16_t t){
    TIMSK |= _BV(OCIE1A);
    OCR1A = t;
}

//pickup
void pickup(){
    t0 = TCNT1;
    TCNT1 = 0;
    uint8_t X = PIND&_BV(INP);

    if(timeout){
        UART_TxChar('H');
        timeout = false;
        _delay_ms(100);
        return;
    }
 
    
    uint16_t rpm = 8437500 / t0;

    setTimer(t0/8);
    if(rpm > 2000){
        /* setTimer((uint32_t)t0*50/360); */
        /* setTimer(t0); */
    }
    else{
        /* setTimer((uint32_t)t0*50/360); */
    }

    /* DEBUG */
    UART_TxChar('T');
    UART_TxChar(t0/256);
    UART_TxChar(t0%256);
    UART_TxChar(0);
    UART_TxChar(0);
    /* DEBUG */
}

int main(void){
    OSCCAL = 0x9f;
    UART_Init();
    UART_SetBaudRate(38400);

	//pins init
	DDRB = _BV(OUTP);
    PORTB = 0;
	PORTD = _BV(INP);
    
	TCCR1B = _BV(CS00) | _BV(CS01); //prescaler /64
	TIMSK |= _BV(TOIE1); //enable overflow interrupt
	sei();
    
    UART_TxChar('A');

    uint8_t ready = 1;



    uint16_t startT = 0;
    uint16_t tt = 0;
    while(true){
        tt = TCNT1 - startT;
        /* UART_TxChar('T'); */
        /* UART_TxChar(0); */
        /* UART_TxChar(2); */
        /* UART_TxChar(tt/256); */
        /* UART_TxChar(tt%256); */
        if(tt > 1000)
        {
            pickup();
        }
        startT = TCNT1;
        {
            int cc = 0;
            while(true) //ON
            {
                if((PIND&_BV(INP))){
                    cc++;
                    if(cc==30){
                        break;
                    }
                }
                else{
                    if(cc>0){
                        cc--;
                    }
                }
            }
        }
        tt = TCNT1 - startT;
        /* UART_TxChar('T'); */
        /* UART_TxChar(0); */
        /* UART_TxChar(1); */
        /* UART_TxChar(tt/256); */
        /* UART_TxChar(tt%256); */
        startT = TCNT1;
        {
            int cc = 0;
            while(true) //ON
            {
                if((PIND&_BV(INP))){
                    if(cc>0){
                        cc--;
                    }
                }
                else{
                    cc++;
                    if(cc==30){
                        break;
                    }
                }
            }
        }
    }
}

ISR(TIMER1_OVF_vect)
{
    timeout = 1;
    t0 = 0;
}
ISR(TIMER1_COMPA_vect){
    spark();
    TIMSK ^= _BV(OCIE1A);
}

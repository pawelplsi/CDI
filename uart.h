/**
 * Copyright (c) 2016, Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 * Software UART for ATtiny13
 */

#ifndef	_UART_H_
#define	_UART_H_

//#define	UART_RX_ENABLED		(1) // Enable UART RX
#define	UART_TX_ENABLED		(1) // Enable UART TX


# define        UART_BAUDRATE   (192000)

#define	TXDELAY         	(int)(((F_CPU/UART_BAUDRATE)-7 +1.5)/3)
#define RXDELAY         	(int)(((F_CPU/UART_BAUDRATE)-5 +1.5)/3)
#define RXDELAY2        	(int)((RXDELAY*1.5)-2.5)
#define RXROUNDED       	(((F_CPU/UART_BAUDRATE)-5 +2)/3)
#if RXROUNDED > 127
# error Low baud rates are not supported - use higher, UART_BAUDRATE
#endif

char uart_getc(void);
void uart_putc(char c);
void uart_putu(uint16_t x);
void uart_puts(const char *s);

#endif	/* !_UART_H_ */


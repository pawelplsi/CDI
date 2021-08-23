MCU=atmega8
F_CPU=8000000
CC=avr-gcc
LD=avr-ld
OBJCOPY=avr-objcopy
SIZE=avr-size
AVRDUDE=avrdude
CFLAGS=-Wall -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.
TARGET=main

SRCS=main.c 

all:
	${CC} ${CFLAGS} -o ${TARGET}.o ${SRCS} -c
	${LD} -o ${TARGET}.elf ${TARGET}.o
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.o ${TARGET}.hex
	${SIZE} -C --mcu=${MCU} ${TARGET}.elf

flash:
	${AVRDUDE} -p ${MCU} -c avrisp -b 19200 -U flash:w:${TARGET}.hex:i -P /dev/ttyUSB0

clean:
	rm -f *.c~ *.o *.elf *.hex

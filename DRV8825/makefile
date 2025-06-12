#Siddharth Vasudevan
#Makefile for component testing
#Created 6/2/25

#Comments
#--------
#6/12/25: Modified filename, will include library for DRV8825.


#-----Constants-----
SERIALPORT = COM3 #required for flashing to ATMega328p
CLOCKSPEED = 16000000	
PROGRAMMER = arduino


#-----Relevant files-----
SOURCEFILE = main.c lib/uart.c #main .c file + libraries .c files


#-----Recipes-----
begin:	main.hex #entry point set to main.hex recipe

main.hex: main.elf #copies files into .elf and .hex formats
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=atmega328p main.elf 

main.elf: $(SOURCEFILE) 
	avr-gcc -Wall -Os -DF_CPU=$(CLOCKSPEED) -mmcu=atmega328p -o main.elf $(SOURCEFILE)

flash:	begin #flash to ATMega
	avrdude -c $(PROGRAMMER) -b 115200 -P $(SERIALPORT) -p atmega328p -U flash:w:main.hex:i

clean: #remove intermediate files
	rm -f main.elf main.hex


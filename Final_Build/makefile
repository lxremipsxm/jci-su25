#Siddharth Vasudevan
#Makefile for component testing
#Created 6/30/25

#Comments
#--------
#6/30/25: Copied over from DRV8825/ directory. Made modifications for ATmega2560


#-----Constants-----
SERIALPORT =  COM4 #required for flashing to ATmega
CLOCKSPEED = 16000000	
PROGRAMMER = arduino
AVRDUDE = "C:\Users\siddv\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17\bin\avrdude.exe"
AVRDUDE_CONF = "C:\Users\siddv\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17\etc\avrdude.conf"


#-----Relevant files-----
SOURCEFILE = main.c lib/uart.c lib/drv8825_2560.c lib/servo.c lib/SSD1306.c lib/i2c.c#main .c file + libraries .c files


#-----Recipes-----
begin:	main.hex #entry point set to main.hex recipe

main.hex: main.elf #copies files into .elf and .hex formats
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=atmega2560 main.elf 

main.elf: $(SOURCEFILE) 
	avr-gcc -std=c99 -Wall -Os -DF_CPU=$(CLOCKSPEED) -mmcu=atmega2560 -o main.elf $(SOURCEFILE)

flash:	begin #flash to ATMega
	$(AVRDUDE) -C $(AVRDUDE_CONF) -v -c wiring -p atmega2560 -P COM4 -b 115200 -D -U flash:w:main.hex:i

clean: #remove intermediate files
	rm -f main.elf main.hex


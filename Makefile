ARMGNU ?= arm-none-eabi

AOPS = --warn --fatal-warnings
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding

all : kernel.img

clean :
	rm -f *.o
	rm -f *.srec
	rm -f *.elf
	rm -f *.list
	rm -f *.img

init.o : init.s
	$(ARMGNU)-as $(AOPS) init.s -o init.o

blink.o : blink.c 
	$(ARMGNU)-gcc $(COPS) -c blink.c -o blink.o

blink.elf : memmap blink.o init.o
	$(ARMGNU)-ld init.o blink.o -T memmap -o blink.elf
	$(ARMGNU)-objdump -D blink.elf > blink.list

kernel.img : blink.elf
	$(ARMGNU)-objcopy --srec-forceS3 blink.elf -O srec blink.srec
	$(ARMGNU)-objcopy blink.elf -O binary kernel.img

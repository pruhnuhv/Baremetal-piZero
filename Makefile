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


main.o : main.c
	$(ARMGNU)-gcc $(COPS) -c main.c -o main.o

main.elf : memmap main.o
	$(ARMGNU)-ld main.o -T memmap -o main.elf
	$(ARMGNU)-objdump -D main.elf > main.list

kernel.img : main.elf
	$(ARMGNU)-objcopy --srec-forceS3 main.elf -O srec main.srec
	$(ARMGNU)-objcopy main.elf -O binary kernel.img

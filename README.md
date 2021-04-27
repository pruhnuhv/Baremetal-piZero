#Blinking The ACT LED on a Rpi Zero

This is a basic baremetal program to blink the inbuilt ACT LED on the Raspberry Pi Zero, while making use of the System Timer on the device.

On power on, the GPU loads the bootcode.bin file (cloned from the raspberrypi firmware repository) into memory, which inturn searches the for a start.elf file on the SD Card, and then for a kernel.img file which it loads into the memory at 0x8000. Here, we use the cross compiler and linker to create our own kernel.img from the code we have written and the linker script ensures that the function, blink() in this case loads at 0x8000 in the memory.

The Raspberry Pi Zero has a BCM2835 chip on board. (The ACT LED on the pi zero is connected to the GPIO47 pin)
Now, according to the BCM2835 documentation, the GPIO base register exists at 0x20200000 (ARM Address) along with 6 Function select registers to define the functions of the 53 GPIO Registers onboard. The GPFSEL0 is meant for the GPIO pins 0-9, and so on. Correspondingly, for our program, we require the GPFSEL4 register which defines the functions of the GPIO pins 40-49. Here, each pin has 3 dedicated bits for us to work with on the 32 bit (with 2 reserved bits) GPIO Function select registers. Setting these 3 bits to 000 would mean that the corresponding GPIO Pin is an input and 001 would mean that it is an output, 010 and ahead is used to define alternate functions and we do not need this in the limited scope of this program. 
So, to set the GPIO Pin 47 as an output, we need to modify the 23-21 bits to 001, while keeping the other bits unchanged.

Since the raspberry pi has a 1Mhz timer, 0x00500000 ticks take approximately 5 seconds(5.2 seconds to be precise). The wait() function makes use of the Lower 32 bits of the System Timer (0x20003000+0x4) to waste time when called.

Following this, now we need need to actually blink the LED. BCM2835 has dedicated 2 pairs of Set and Clear registers for the same. Here, the 32 bit register, GPCLR0 is used to clear the first 32 GPIO Pins ie. GPIO 0-31. For our program we need the GPCLR1 and similarly the GPSET1 registers for GPIO47 (if GPIO 32 is the 0th bit, GPIO47 pin corresponds to the 15th bit on the GPSEL1, GPCLR1 registers). As the program has no Operating System to return to, we let the function run in an infinite while loop, wherein the LED stays on for 5 seconds and then gets turned off for the next 5 seconds and so on.

References: 
* [BCM2835 Manual](https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf)
* [Baking Pi](https://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/index.html)
* [dwelch67/raspberrypi](https://github.com/dwelch67/raspberrypi)

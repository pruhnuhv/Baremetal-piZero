# Controlling The ACT LED Using UART

With reference to the basic blinking ACT LED and the UART register definitions in the BCM2835, we enable the Mini UART on the Raspberry Pi Zero. We receive a Starting confirmation from the Pi on our terminal Emulator (I have use Minicom and GTKTerm), following which we can send in the 'f' character to switch on the LED, and the 'e' character to consequently switch it off.

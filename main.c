
#define GPFSEL3 0x2020000C
#define GPFSEL4 0x20200010
#define GPSET1  0x20200020
#define GPCLR1  0x2020002C
#define total_time 0x00500000
#define SYSTIMERCLO 0x20003004


volatile unsigned int* systimer = (unsigned int*)(SYSTIMERCLO);
volatile unsigned int* reg = (unsigned int*)(GPFSEL4);
volatile unsigned int* LED_GPSET = (unsigned int*)(GPSET1);
volatile unsigned int* LED_GPCLR = (unsigned int*)(GPCLR1);

int blink (void);
void wait(void);

int blink ( void )
{
  /*
   * GPFSEL4 controls GPIO pins 40-49, Here, the ACT LED on the Raspberry Pi is connected to the GPIO 47 pin.
   * We need to modify the 32 bit GPFSEL4 Register in such a way that it sets the GPIO47 pin as an output.
   * Here, each pin takes 3 bits, so correspondingly, to set GPIO 47 to output (001), (47-40 =7 ; 7*3 = 21, so 23-21 needs to be set to 001)
   */
  *reg &= ~(7<<21);   //clearing the 21-23 bits, while the other bits remain unchanged
  *reg |= (1<<21);    //setting the 21st bit

  /*
   * The GPSET1/GPCLR1 32 bit registers are used to set and clear the 32nd to the 53rd GPIO pins. Here, the 0th bit is for GPIO 32,
   * therefore, we need to modify (set) the bit no. 15 which is for GPIO 47 on the GPSET1 and GPCLR1 registers. 
   */
  while(1)
  {
    *LED_GPSET = 1<<(15); //Setting the 15th bit (47-32)
    wait();
    *LED_GPCLR = 1<<(15); //Setting the 15th bit (47-32)
    wait();
  }
  return(0);
}

void wait(void){
  unsigned int temp = *systimer+total_time;
  while(1)
  {
    if(temp == *systimer) break;
  }
}

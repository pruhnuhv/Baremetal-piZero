

#define GPFSEL1 0x20200004
#define GPSET0  0x2020001C
#define GPCLR0  0x20200028
#define GPPUD       0x20200094
#define GPPUDCLK0   0x20200098
#define AUX_ENABLES     0x20215004
#define AUX_MU_IO_REG   0x20215040
#define AUX_MU_IER_REG  0x20215044
#define AUX_MU_IIR_REG  0x20215048
#define AUX_MU_LCR_REG  0x2021504C
#define AUX_MU_MCR_REG  0x20215050
#define AUX_MU_LSR_REG  0x20215054
#define AUX_MU_MSR_REG  0x20215058
#define AUX_MU_SCRATCH  0x2021505C
#define AUX_MU_CNTL_REG 0x20215060
#define AUX_MU_STAT_REG 0x20215064
#define AUX_MU_BAUD_REG 0x20215068
#define GPFSEL4 0x20200010
#define GPSET1  0x20200020
#define GPCLR1  0x2020002C

volatile unsigned int* GPFSEL_reg = (unsigned int*)GPFSEL1;
volatile unsigned int* GPCLR_reg = (unsigned int*)GPCLR0;
volatile unsigned int* GPSET_reg = (unsigned int*)GPSET0;
volatile unsigned int* GPPUD_reg = (unsigned int*)GPPUD;
volatile unsigned int* GPPUDCLK0_reg = (unsigned int*)GPPUDCLK0;
volatile unsigned int* uart_enables = (unsigned int*)AUX_ENABLES;
volatile unsigned int* uart_iodata = (unsigned int*)AUX_MU_IO_REG;
volatile unsigned int* uart_interrupt_enable = (unsigned int*)AUX_MU_IER_REG;
volatile unsigned int* uart_interrupt_identify  = (unsigned int*)AUX_MU_IIR_REG;
volatile unsigned int* uart_lcr  = (unsigned int*)AUX_MU_LCR_REG;
volatile unsigned int* uart_mcr = (unsigned int*)AUX_MU_MCR_REG;
volatile unsigned int* uart_lsr = (unsigned int*)AUX_MU_LSR_REG;
volatile unsigned int* uart_msr = (unsigned int*)AUX_MU_MSR_REG;
volatile unsigned int* uart_scratch = (unsigned int*)AUX_MU_SCRATCH;
volatile unsigned int* uart_cntl = (unsigned int*)AUX_MU_CNTL_REG;
volatile unsigned int* uart_stat = (unsigned int*)AUX_MU_STAT_REG;
volatile unsigned int* uart_baud = (unsigned int*)AUX_MU_BAUD_REG;
volatile unsigned int* LED_reg = (unsigned int*)(GPFSEL4);
volatile unsigned int* LED_GPSET = (unsigned int*)(GPSET1);
volatile unsigned int* LED_GPCLR = (unsigned int*)(GPCLR1);


void uart_send_character (char c)
{
    while(1) {
      if((*uart_lsr)&0x20 ) break;
    }
    *uart_iodata = c;
}

void uart_send_string (char* str)
{
    for (int i = 0; str[i] != '\0'; i ++) {
        uart_send_character((char)str[i]);
    }
}

char uart_receive (void)
{
    while(1) {
      if((*uart_lsr)&0x01){
        break;
      } 
    }
    return((*uart_iodata)&0xFF);
}


void blink(){
    
  *uart_enables = 1;  //Enable Mini-UART
  *uart_cntl = 0; //Disable auto flow, receiver & transmitter
  *uart_interrupt_enable = 0; //Disable recevie & transmit interrupts
  *uart_lcr = 3;  //UART works in 8 bit mode (referred BCM2835 errata)
  *uart_mcr = 0;  //Request to Send set to 1
  *uart_baud = 270; //Baud rate set to 115200
  *uart_cntl = 3; //Enable transmitter and receiver
  
 
  *GPFSEL_reg &= ~(15<<12);  //Clearing the 6 bits for GPIO 14,15
  *GPFSEL_reg |= (18<<12);   //0b010010 = 0d18, setting both to ALT Fn5

  *LED_reg &= ~(7<<21);       
  *LED_reg |= (1<<21);
  
  uart_send_string("START ");
  while(1)
  {
    unsigned int temp;
    temp = uart_receive();
    if(temp == 0x65){
      *LED_GPSET = 1<<(15);
      uart_send_string(" LED OFF ");
    }
    if(temp == 0x66){
      *LED_GPCLR = 1<<(15);
      uart_send_string(" LED ON ");
    }
  }
}



/*
 * File name            : uart.c
 * Compiler             : MPLAB XC8/ MPLAB C18 compiler
 * IDE                  : Microchip  MPLAB X IDE v1.90/ MPLAB IDE v8
 * Processor            : PIC18
 * Author               : Etiq Technologies
 * Created on           : October 30, 2013, 1:20 PM
 * Description          : UART functions definitions source file
 */
#if defined(__XC)
    #include <plib\usart.h>
#elif defined(__18CXX)
	#include <p18cxxx.h>   /* C18 General Include File 		*/
    #include <usart.h>
#endif
/*______________________________ UART functions_____________________________________*/


void UsartConfig(unsigned char config1, unsigned char config2)
    {
     OpenUSART( config1, config2);
    }

void Write(char data)
{
    WriteUSART(data);
    while(BusyUSART());
}

void Write_str(unsigned char *c)                                    /* Function writes a string to UART                      */
    {
    Write(0x0D);
    while(*c != '\0')
        {
        WriteUSART(*c);
        while(BusyUSART());
        c++;
        }
    }

unsigned char Read_char(void)                                        /* Function reads a byte from UART                      */
    {
    unsigned char c;
    while(!(DataRdyUSART()));
    c = ReadUSART();
    Write('\n');
    Write('\r');
    Write(c);
    return (c);
    }
unsigned char Read(void)
{
while(!(DataRdyUSART()));                                           /* Reading a data byte                                        */
     return(ReadUSART());
}

void Read_string(unsigned char *buffer, unsigned char len)            /* Reads a string data from UART of having specific length*/
  {
  char i;                                                             
  unsigned char data; 
      Write(0x0D);
      Write('\n');
  for(i=0;i<len;i++)                                                  
    {
    while(!DataRdyUSART());                                           /* Wait for data to be received                           */
    data = getcUSART();                                               /* Get a character from the USART                         */
    *buffer = data;                                                   /* save data in a string                                  */
    WriteUSART(data);
    while(BusyUSART());
    buffer++;                                                         /* Increment the string pointer                           */
    }
  }

void Config_Interrupt_Rx(void)
{
    PIR1bits.RCIF = 0;                                                   /* Reset EUSART Receive Interrupt Flag                  */
    IPR1bits.RCIP = 0;                                                   /* Low priority for EUSART Receive Interrupt            */
    PIE1bits.RCIE = 1;                                                   /* RX interrupt Enable                                  */
    INTCONbits.PEIE = 1;                                               /* Peripheral Interrupt Enable                          */
}



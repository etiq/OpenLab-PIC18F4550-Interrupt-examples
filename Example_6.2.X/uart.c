
/*
 * File name            : uart.c
 * Compiler             : MPLAB XC8/ MPLAB C18 compiler
 * IDE                  : Microchip  MPLAB X IDE v1.90/ MPLAB IDE v8
 * Processor            : PIC18
 * Author               : Etiq Technologies
 * Created on           : October 30, 2013, 2:13PM
 * Description          : UART function definitions source file
 */

#if defined(__XC)
    #include <plib\usart.h>
#elif defined(__18CXX)
	#include <p18cxxx.h>   /* C18 General Include File 		*/
    #include <usart.h>

#endif
/*______________________________      UART functions       _____________________________________*/


void UsartConfig(unsigned char config1, unsigned char config2)      /* UART configuration function                          */
    {
    OpenUSART( config1, config2);
    }

void Write(char data)                                               /* UART single byte write function                      */
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
unsigned char Read_char(void)                                        /* Function to read a byte through UART                  */
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
while(!(DataRdyUSART()));                                             /* Reading a data byte                                        */
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
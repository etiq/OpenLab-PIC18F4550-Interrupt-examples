#include <plib\usart.h>


//////////////////////////////////////  USART FUNCTIONS  //////////////////////////////////////

void UsartConfig(void)
    {
    OpenUSART( (USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_LOW), 0x1F );
    }

void Write(char data)
{
    WriteUSART(data);
    while(BusyUSART());
}

void Write_str(unsigned char *c)
    {
    while(*c != '\0')
        {
        WriteUSART(*c);
        while(BusyUSART());
        c++;
        }
    }
void Read_str(unsigned char *str, unsigned char n)
    {
    while(!(DataRdyUSART()));                                     /* configure external LCD                                      */
    getsUSART((char*)str, n);
    }
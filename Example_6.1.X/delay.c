/*
 * File name            : delay.c
 * Compiler             : MPLAB XC8/ MPLAB C18 compiler
 * IDE                  : Microchip  MPLAB X IDE v1.90/ MPLAB IDE v8
 * Processor            : PIC18
 * Author               : Etiq Technologies
 * Created on           : November 27, 2013, 01:13 PM
 * Description          : Delay function definitions source file
 */


#if defined(__18CXX)
#include <delays.h>
#elif defined(__XC)
#include <plib\delays.h>
#endif

void delay_ms(unsigned char t)
{
    unsigned char i;
    for (i=0;i<=t;i++)
       Delay1KTCYx(5);                                          /* Delay of 1ms                                                     */
                                                                /* Cycles = (TimeDelay * Fosc) / 4                                  */
                                                                /* Cycles = (1ms * 20MHz) / 4                                       */
                                                                /* Cycles = 5,000                                                   */
  return;
}
void delay_us(unsigned char t)
{
    unsigned char i;                                            /* Delay of 1us                                                     */
    for (i=0;i<=t;i++)                                          /* Delay multiples of 10 Tcy                                        */
        Delay1KTCYx(1);                                          /* Cycles = (TimeDelay * Fosc) / 4                                  */
                                                                /* Cycles = (1ms * 20MHz) / 4                                       */
                                                                /* Cycles = 5                                                       */
    return;
}
void Delay_s(unsigned char t)                                   /* Delay of t seconds                                               */
    {
    unsigned char i,j;                                          /* 10 * 100msec = 1000msec (1second) for 10 cycles of inner loop    */
    for(i=0;i<t;i++)                                            /* Delay by t cycles = t seconds                                    */
        for(j=0;j<10;j++)
            delay_ms(100);                                      /* 100msec delay                                                    */
    }

void Delay_ms_(unsigned char t)                                   /* Delay of t seconds                                               */
    {
    unsigned char i,j;                                          /* 10 * 100msec = 1000msec (1second) for 10 cycles of inner loop    */
    for(i=0;i<t;i++)                                            /* Delay by t cycles = t seconds                                    */
        for(j=0;j<5;j++)
            delay_ms(100);                                      /* 100msec delay                                                    */
    }

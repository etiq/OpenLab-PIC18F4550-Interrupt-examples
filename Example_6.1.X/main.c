/*
 * File name            : main.c
 * Compiler             : MPLAB XC8/ MPLAB C18 compiler
 * IDE                  : Microchip  MPLAB X IDE v1.90/ MPLAB IDE v8
 * Author               : Etiq Technologies
 * Processor            : PIC18
 * Description          : Example_6.1 External Interrupt
 * Created on           : January 7, 2014, 01:18 PM
 *                      : Example code Configures External interrupt INT0 for demonstrating the transfer of processor
 *                        control in between main routine and subroutine.
 */

/*
 * Example code demonstrates
 *  1. Configuration of external interrupt.
 *  2. Execution of interrupt service rouine.
 *
 * PORT CONNECTIONS
 * ----------------
 * Output - Connector J3 of IO board_1 to PORTD on base board.
 * Input  - External Interrupt at pin RB0 on base board.
 *
 * OPERATION
 * ---------
 * On POR(Power on reset) the PORTD bit RD0 blinks till any external interrupt occures.
 *
 * 1. Interrupt the processor at external interrupt pin INT0 (RB0).
 * 2. LED connected to RD1 starts blinking, that indicates processor control is transferred to isr.
 * 3. Execution returns to the main routine and continues execution.
 *    User can write a differnt application code in isr.
 */

/* Interrupt on rising edge*/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
    #include <adc.h>
#endif

#if defined(__XC) || defined(HI_TECH_C)
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#endif

#if defined(__XC) || defined(__18CXX)
#include "config.h"
#include "delay.h"
#endif

#define _XTAL_FREQ 20000000
#define USE_AND_MASKS

void Config_Intrrupt_Ext(void)
{
    INTCONbits.INT0IE  = 1;                                       /* External interrupt enable                                  */
    INTCONbits.INT0IF == 0;                                       /* Reset external interrupt flag bit                          */
    INTCONbits.PEIE = 1;                                          /* Peripheral Interrupt Enable                                */
}


#if defined(__18CXX)
/* ______________________________________C18 ISR________________________________________________*/
void Interrupt_Ext(void);

#pragma code HIGH_INTERRUPT_VECTOR = 0x08
void high_ISR (void)
{
_asm
goto Interrupt_Ext
_endasm
}
#pragma code                                                        /* allow the linker to locate the remaining code */

#pragma interrupt Interrupt_Ext
void Interrupt_Ext(void)
{
 unsigned char i;
     if(INTCONbits.INT0IF)                                          /* Testing external interrupt flag is set or not               */
    {
        LATDbits.LATD0 = 0;
        for(i=0;i<10;i++)                                           /* Looping 10 cycles                                           */
        {
        LATDbits.LATD1 = ~PORTDbits.RD1;                           /* Logical NOT operation on bit RD1                             */
        Delay_ms_(1);                                              /* Delay function call                                         */
        }                                                           
    INTCONbits.INT0IF =0;                                           /* Reset external interrupt flag bit                           */
    }
}

void ei(void)
{
INTCONbits.GIE = 1;										/* Global interrupt enable								 */
}
#elif defined(__XC)
/*______________________Interrupt Service Routine______________________________________________*/

#pragma interrupt_level 0 @0x0018
 void interrupt Ext_isr()                                           /* Interrupt service routine for Tmer0 interrupt               */
    {
     unsigned char i;
     if(INTCONbits.INT0IF)                                          /* Testing external interrupt flag is set or not               */
    {
        LATDbits.LATD0 = 0;
        for(i=0;i<10;i++)                                           /* Looping 10 cycles                                           */
        {
        LATDbits.LATD1 = ~PORTDbits.RD1;                           /* Logical NOT operation on bit RD1                            */
        Delay_ms_(1);                                               /* Delay function call                                         */
        }                                                           
    INTCONbits.INT0IF =0;                                           /* Reset external interrupt flag bit                           */
    }
    }
#endif
/*____________________________________main routine_______________________________________________*/

int main(void)
{
    UCONbits.USBEN = 0;                                           /* Disabling USB module                                       */
    RCONbits.IPEN = 0;                                            /* Disabling Priority on Interrupts                           */
    ADCON1  = 0x0F;                                               /* AN0 Through AN12 Configured as Digital I/O                 */
    TRISDbits.TRISD1 = 0;                                         /* Direction of bit RD1 as output                             */
    TRISDbits.TRISD0 = 0;                                         /* Direction of bit RD0 as output                             */
    LATDbits.LATD0 = 0;                                           /* Clearing bit LATD0 of LATD register                        */
    LATDbits.LATD1 = 0;                                           /* Clearing bit LATD1 of LATD register                        */
    Config_Intrrupt_Ext();
    ei();                                                         /* Global interrupt enable                                    */
 while(1)                                                         /* Infinite loop                                              */
    {
     LATDbits.LATD0 = ~PORTDbits.RD0;                             /* Logical NOT operation on bit RD0                           */
     Delay_ms_(1);                                                /* Delay function call                                        */
    }                                                             
return (0);
}
/*
 * File name            : main.c
 * Compiler             : MPLAB XC8/ MPLAB C18 compiler
 * IDE                  : Microchip  MPLAB X IDE v1.90/ MPLAB IDE v8
 * Author               : Etiq Technologies
 * Processor            : PIC18
 * Description          : Example_6.3 TIMER0 Interrupt_1
 * Created on           : January 7, 2014, 05:31 PM
 *                      : Example code for TIMER0 Interrupt configuration.
 */

/* Timer0 Peripheral Interrupt
 * TIMER0 configured for generating TMR0 interrupt every seconds. TIMER0 interrupt is enabled with low priority.
 * On each overflow of TMR0 register from 0xFFFF to 0x0000 (in 16bit mode)result in  generation of interrupt and Timer0
 * interrupt flag bit to set. By confirming that interrupt flag is set on each second, bit RB0 output is complemented to
 * indicate the interrupt.
 */
/*
 * PORT connections
 * ----------------
 * 1. PORTB
 *    Output - IO board_2 FRC connector J3 to PORTB FRC connector J22 on base board.
 *
 * 2. USART
 *      J21 DB9 female connenctor of PIC board to Computer serial port.
 */

/*  OPERATION
    ---------
 * 1. Open PC Hyper Terminal. Reset the microcontroller.
 * 2. On POR (Power On Reset) the controller sends information about Interrupt occurred in every seconds.
 * 3. Observe RB0 bit of PORTB output at IO board_2, which blinks in every seconds indicates that, processor
 *    control tranfers to isr in every seconds.
 */

/*
  Calculation of TMR0 register value for 1 sec delay
 *
 * With 20MHz oscillator frequency and 256 prescalar(selected) the delay obtained is 4/(20000000) *256 = 51.2us.
 * On each increment of loaded value in TMR0, 51.2us delay occures. For 1s delay, value count in TMR0 register
 * needed is 1/51.2us = 19531. 1sec delay occur on completion of 19531 increments from the TMR0 value in TMR0 register.
 * Therefore the difference of 19531 from maximum value 65535(0xFFFF) is to be loaded to the TMR0 register.
 *
 * For 1sec delay, TMR0 value = (65535 - 19531) = 46004 (0xB3B4 in hex)
 *
 */

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File 		*/
    #include <plib\timers.h>
    #include <stdlib.h>
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File 	*/
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File 		*/
    #include <timers.h>
    #include <stdlib.h>
#endif

#if defined(__XC) || defined(HI_TECH_C)
#include <stdint.h>        /* For uint8_t definition 		*/
#include <stdbool.h>       /* For true/false definition 	*/
#endif

#if defined(__XC) || defined(__18CXX)
#include "config.h"
#include "delay.h"
#include "uart.h"
#endif

#define USE_AND_MASKS
#define _XTAL_FREQ 20000000
#define baud 9600                                                  /* UART baud rate                                       */
#define config_2 ((_XTAL_FREQ/baud/64)-1)
unsigned int load = 0xB3B4;
unsigned char config_1 = 0x00, str[] = "TMR0 Interrupt in every 1Sec..!\n";


void putch(unsigned char byte);

 void Config_Interrupt_TMR0(void)
 {
    INTCONbits.TMR0IF = 0;                                                   /* Reset Timer0 Interrupt Flag                          */
    INTCON2bits.TMR0IP = 0;                                                   /* Low priority for Timer0 Interrupt                    */
    INTCONbits.TMR0IE = 1;                                                   /* TIMER0 interrupt Enable                              */
    INTCONbits.PEIE = 1;                                                     /* Peripheral Interrupt Enable                          */
 }

 void function(void)
 {
      Write_str(str);            /* Writing a string through UART                               */     
      LATBbits.LATB0 = ~LATBbits.LATB0;                          /* Logical NOT operation on bit RB0                            */
      INTCONbits.TMR0IF = 0;                                     /* clearing Timer0 receive flag                                */
      WriteTimer0(load);                                         /* Reloading the value to the TMR0(TMR0H and TMR0L) register   */
 }

/*_____________________________________ C18 compiler ISR __________________________________________________*/

#if defined(__18CXX)

void TMR0_isr (void); 							    /* prototype for 'goto' specified function	 			 */
#pragma interruptlow TMR0_isr
void TMR0_isr (void)
{
   if(INTCONbits.TMR0IF)                                                    /* Testing Timer0 interrupt flag is set or not                 */
    function();                                                             /* function call                                               */
}

#pragma code low_vector_section=0x08                                        /* control is transferred to address 0x08				 */
void low_vector (void)
{
_asm
GOTO TMR0_isr
_endasm
}
#pragma code                                                                /* allow the linker to locate the remaining code 		 */

void ei(void)
{
INTCONbits.GIE = 1;                                                         /* Global interrupt enable								 */
}
 #elif defined(__XC)

/*______________________          Interrupt Service Routine           ___________________________________*/

void interrupt Timer0_isr()                                        /* Interrupt service routine for Tmer0 interrupt               */
    {
    if(INTCONbits.TMR0IF)                                          /* Testing Timer0 interrupt flag is set or not                 */
    function();                                                    /* function call                                               */
    }
 #endif


void putch(unsigned char byte)
{
Write(byte);
}

/* ______________________________________main Routine_______________________________________*/
int main(void)
{
unsigned char config_3=0x00;
config_1 = (USART_TX_INT_OFF  & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_LOW);
TRISBbits.TRISB0 = 0;                                               /* Direction of the bit RB0 as output                   */
LATBbits.LATB0 = 0;
Config_Interrupt_TMR0();                                                               /* Global interrupt enable                              */
config_3 = T0_16BIT & T0_SOURCE_INT & T0_PS_1_256 & TIMER_INT_ON;   /* Timer0 configuration                                 */
WriteTimer0(0x00);                                                  /* clear timer if previously contains any value         */
UsartConfig(config_1, config_2);                                                      /* invoking USART configuration function                */
OpenTimer0(config_3);                                               /* Invoking Timer0 configuration function               */
ei();
WriteTimer0(load);                                                  /* Invoking function to write value to TMR0 register    */
while(1);                                                           /* Infinite loop                                        */
return (0);
}
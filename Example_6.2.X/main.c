/*
 * File name            : main.c
 * Compiler             : MPLAB XC8/ MPLAB C18 compiler
 * IDE                  : Microchip  MPLAB X IDE v1.90/ MPLAB IDE v8
 * Author               : Etiq Technologies
 * Processor            : PIC18
 * Description          : Example_6.2 PORTB Interrupt on change
 * Created on 			: January 08, 2014, 12:50 PM
 *                      : Example code for Configuring PORTB change interrupt.
 */


/* In below example code, PORTB change Interrupt is enabled for execution of a subroutine. An input
 * change on bits of PORTB<7:4> interrupts the execution of main routine and the processor tranfers
 * its control to isr and call a function to send an indication through UART as the PORTB change
 * Interrupt happened. User can write different codes in the sub routine as desired.
 */

/*
 * PORT connections
 * ----------------
 * 1. PORTD
 *    Output - IO board_1 FRC connector J3 to PORTD FRC connector J21 on base board.
 * 2. PORTB
 *    Output - IO board_2 FRC connector J3 to PORTB FRC connector J22 on base board.
 * 3. USART
 *      J21 DB9 female connenctor of PIC board to Computer serial port.
 */

/*  OPERATION
    ---------
 * 1. Open PC Hyper Terminal. Reset the microcontroller.
 * 2. Interrupt the processor (Apply a logic HIGH input to any of the pins PORTB<7:4>)to transfer its control to the subroutine.
 * 3. Pin RD1 of PORTD blinks for 1sec as the control transfers to isr.
 */
#include "config.h"
#include "delay.h"
#include "uart.h"

#define _XTAL_FREQ 20000000
#define baud 9600                                                  /* UART baud rate                                       */
#define config2 ((_XTAL_FREQ/baud/64)-1)
unsigned char config1 = 0x00, str_1[19] = "\nPORTB interrupt..!";

void Config_interruptPORTB(void)
{
    INTCONbits.RBIF = 0;                                        /* Reset RB PORT change Interrupt Flag bit              */
    INTCON2bits.RBIP= 1;                                       /* Low priority for RB PORT change Interrupt            */
    INTCONbits.RBIE = 1;                                        /* RB PORT change Interrupt Enable                      */
    INTCONbits.PEIE = 1;                                        /* Peripheral Interrupt Enable                          */
}

void function(void)                                                  
    {
    unsigned char i;                                                    
    Write_str(str_1);                                  /* Writing a string through UART                        */
    LATDbits.LATD0 = 0;
    for(i=0;i<5;i++)                                                    /* Looping 4 cycles                                     */
        {
        LATDbits.LATD1 = ~PORTDbits.RD1;                                /* Logical NOT operation on RA0                         */
        delay_ms(200);                                                     /* 200msec delay                                        */
        }
    LATDbits.LATD1 = 0;
    }
#if defined(__18CXX)
/* ______________________________________ C18 ISR ________________________________________________*/
void Interrupt_PORTB(void);

#pragma code LOW_INTERRUPT_VECTOR = 0x08
void LOW_ISR (void)
{
_asm
goto Interrupt_PORTB
_endasm
}
#pragma code /* allow the linker to locate the remaining code */

#pragma interrupt Interrupt_PORTB
void Interrupt_PORTB(void)
{
  unsigned char c;
    if(INTCONbits.RBIF)                                                     /* Test for TRUE condition of PORTB change Interrupt flag bit   */
    function();                                                             /* Function call                                                */
    c = PORTB;                                                              /* Reading PORTB (RBIF clearing procedure)                      */
    INTCONbits.RBIF = 0;                                                    /* clearing PORTB change Interrupt flag bit                     */

}

void ei(void)
{
INTCONbits.GIE = 1;										/* Global interrupt enable								 */
}
#elif defined(__XC)
/*______________________ Interrupt Service Routine (for XC8 compiler)  _____________________________*/
#pragma interrupt_level 0 @0x0018
void interrupt PORTB_isr()                                                  /* Interrupt service routine for USART receive interrupt        */
{
    unsigned char c;
    if(INTCONbits.RBIF)                                                     /* Test for TRUE condition of PORTB change Interrupt flag bit   */
    function();                                                             /* Function call                                                */
    c = PORTB;                                                              /* Reading PORTB (RBIF clearing procedure)                      */
    INTCONbits.RBIF = 0;                                                    /* clearing PORTB change Interrupt flag bit                     */
}

#endif

/*____________________________  Main Routine _______________________________________________________*/

void main(void)
{
    TRISB = 0xFF;                                               /* Direction of PORTB as input                          */
    ADCON1 = 0x0F;                                              /* AN0 through AN12 configured as Digital I/O           */
    TRISDbits.TRISD0 = 0;                                       /* Direction of bit RD0 as output                       */
    TRISDbits.TRISD1 = 0;                                       /* Direction of bit RD1 as output                       */
    config1 = (USART_TX_INT_OFF  & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_LOW);
    Config_interruptPORTB();
    UsartConfig(config1, config2);                              /* UART configuration function call                     */
    ei();                                                       /* Global Interrupt Enable                              */
    while(1)                                                    /* Infinite loop                                        */
        {
        LATDbits.LATD0 = ~PORTDbits.RD0;                        /* Logical NOT operation on RD0                         */
        delay_ms(200);                                          /* 200msec delay                                        */
        }
}
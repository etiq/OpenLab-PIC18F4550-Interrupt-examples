/*
 * File name            : main.c
 * Compiler             : MPLAB XC8/ MPLAB C18 compiler
 * IDE                  : Microchip  MPLAB X IDE v1.90/ MPLAB IDE v8
 * Author               : Etiq Technologies
 * Processor            : PIC18
 * Description          : Example_6.4 UART Receive Interrupt
 * Created on           : January 03, 2014, 10:17 AM
 *                      : Example code for Configuring UART receive interrupt.
 */

/* Example_6.4 demonstrates,
 * 1. Configuration of UART Receive Interrupt
 * 2. Transfer of processor control to another sub routine and return to main routine.
 */

/*
 * PORT connections
 * ----------------
 * 1. PORTB
 *    Output - FRC connector J3 of IO board_2  to PORTB FRC connector J22 on base board.
 *
 * 2. PORTD
 *    Output - FRC connector J3 of IO board_1  to PORTD FRC connector J21 on base board.
 *
 * 3. USART
 *      J21 DB9 female connenctor of PIC board to Computer serial port.
 */

/*  OPERATION
    ---------
 * 1. Open PC Hyper Terminal. Reset the microcontroller.
 * 2. Observe RD0 bit of PORTD output at IO board_1, blinking LED indicates processor control is in main routine.
 * 3. Listen to terminal and write any character on the window.
 * 4. The controller sends back the information about Interrupt occurred in every data reception.
 * 4. Observe RB0 bit of PORTB output at IO board_2, which blinks in every data reception indicates that, processor
 *    control tranfers to isr.
 */

#include "config.h"
#include "delay.h"
#include "uart.h"

#define _XTAL_FREQ 20000000
#define baud 9600                                                       /* UART baud rate                                       */
#define config_2 ((_XTAL_FREQ/baud/64)-1)
unsigned char c, config_1, str_1[] ="\rPress any Key..", str_2[] = "\nUART receive interrupt..!\n\r";

void function(void)                                                  
    {
    unsigned char i;
    c = Read();                                                         /* Reading from UART buffer register                    */
    Write_str(str_2);                       							/* Writing a string through UART                        */
    Write(c);                                                           /* Writing a character through UART                     */
    LATBbits.LATB0 = 0;                                                 /* Reset output bit RB0                                 */
    for(i=0;i<3;i++)                                                    /* Looping 3 cycles                                     */
        {
        LATBbits.LATB0 = ~PORTBbits.RB0;                                /* Logical NOT operation on RB0                         */
        delay_ms(200);                                                  /* Delay function call for 200 msec                     */
        }
        LATBbits.LATB0 = 0;                                             /* Reset output bit RB0                                 */
    }

/*___________________________________Interrupt Service Routine_________________________________________________*/

#if defined(__XC)

#pragma interrupt_level 0 @0x0018
void interrupt Rx_isr()                                             /* Interrupt service routine for USART receive interrupt        */
{
    if(PIR1bits.RCIF)                                               /* Test for TRUE condition of UART receive interrupt flag bit   */
    function();                                                     /* Function call                                                */
    PIR1bits.RCIF = 0;                                              /* clearing USART receive flag                                  */
}

#elif defined(__18CXX)
void USART_Rx (void); 									/* prototype for 'goto' specified function	 			 */

void ei(void)
{
INTCONbits.GIE = 1;                                     /* Global interrupt enable								 */
}

#pragma interruptlow USART_Rx
void USART_Rx (void)
{
 if(PIR1bits.RCIF == 1)                                 /* Test for USART receive interrupt is set or not        */
   {
	function();                                          /* Invoking function  			                         */
   }
PIR1bits.RCIF = 0;                                      /* clearing USART receive flag                           */
}

#pragma code low_vector_section=0x08  					/* control is transferred to address 0x08				 */
void low_vector (void)									
{
_asm
GOTO USART_Rx
_endasm
}
#pragma code 											/* allow the linker to locate the remaining code 		 */

#endif

/*____________________________________________      Main Routine       ___________________________________________________*/

void main(void)
{
    config_1 = (USART_TX_INT_OFF  & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_LOW);
    TRISDbits.TRISD0 = 0;                                       /* Direction of bit RD0 as output                       */
    TRISBbits.TRISB0 = 0;                                       /* Direction of bit RB0 as output                       */
    UsartConfig(config_1, config_2);                            /* UART configuration function call                     */
    Config_Interrupt_Rx();                                      /* UART receive interrupt initialization function call  */
    ei();                                                       /* Global Interrupt Enable INTCONbits.GIE = 1;          */
    LATDbits.LATD0 = 0;                                         /* Clearing LATD register                               */
    while(1)                                                    /* Infinite loop                                        */
        {
        Write_str(str_1);                         /* UART string witing function call                     */
        LATDbits.LATD0 = ~PORTDbits.RD0;                        /* Logical NOT operation on RD0                         */
        delay_ms(200);                                          /* Delay function call for 200 msec                     */
        }
}

/*
 * File name            : delay.h
 * Compiler             : MPLAB XC8/ MPLAB C18 compiler
 * IDE                  : Microchip  MPLAB X IDE v1.90/ MPLAB IDE v8
 * Processor            : PIC18
 * Author               : Etiq Technologies
 * Created on           : November 27, 2013, 04:02 PM
 * Description          : Delay functions declaration header file
 */

#ifndef DELAY__H
#define	DELAY__H

/*____________________Milli seconds delay function header_________________________________*/
void delay_ms(unsigned char t);
/*
 * Include          :       delay.h
 * Description      :       generates milli seconds delay
 * Arguments        :       Decimal
 * Returns          :       None
 */



/* ______________________Micro seconds delay function header. ____________________________ */
void delay_us(unsigned char t);
/*
 * Include          :       delay.h
 * Description      :       generates micro seconds delay
 * Arguments        :       Decimal
 * Returns          :       None
 */


/* ___________________________Seconds delay function header______________________________ */
void Delay_s(unsigned char t);
/*
 * Include          :       delay.h
 * Description      :       generates seconds delay  
 * Arguments        :       Decimal
 * Returns          :       None
 */

void Delay_ms_(unsigned char t);
#endif	/* DELAY__H */


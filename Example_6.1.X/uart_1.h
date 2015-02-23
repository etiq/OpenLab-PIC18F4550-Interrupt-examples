/* 
 * File:   uart_1.h
 * Author: Etiq technologies
 *
 * Created on November 12, 2013, 5:50 PM
 */

#ifndef UART_1_H
#define	UART_1_H

#ifdef	__cplusplus
extern "C" {
#endif

void Write(char data);
void UsartConfig(void);
void Write_str(unsigned char *c);
#ifdef	__cplusplus
}
#endif

#endif	/* UART_1_H */


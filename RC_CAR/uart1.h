/*
 * uart1.h
 *
 * Created: 2019-07-03 오전 10:24:28
 *  Author: k
 */ 


#ifndef UART1_H_
#define UART1_H_

#define F_CPU	16000000UL
#include <avr/io.h>

#define BAUD 9600


void init_UART1();
void UART1_transmit(unsigned char data);
unsigned char UART1_receive(void);
void UART1_print_string(unsigned char *str);
uint8_t UART1_IsRxD();
uint8_t UART1_IsRxString();
uint8_t* UART1_GetRxString();
void UART1_ISR_Receive();


#endif /* UART0_H_ */
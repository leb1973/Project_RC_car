/*
 * uart1.c
 *
 * Created: 2022-08-08 오후 9:23:06
 *  Author: sikwon
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart1.h"

volatile uint8_t rx1String[64] = {0};
volatile uint8_t rx1ReadyFlag=0;
volatile unsigned char UART1_RX_data;

extern void UART0_transmit(uint8_t data);

ISR(USART1_RX_vect) 	//UART1 수신 인터럽트 서비스 루틴
{
	// UART1_RX_data = UDR1;  // 읽은 다음에 UDR1의 내용이 없어짐 
	UART1_RX_data = UART1_receive();
	
	UART0_transmit(UART1_RX_data);
}

void init_UART1()
{
	UCSR1B |= ((1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1));
	UCSR1A |= (1<<U2X1);
	UBRR1H = 0;
	UBRR1L = 207; // 9600 	
}

void UART1_transmit(unsigned char data)
{	
	while ( !(UCSR1A & (1<<UDRE1)) )
		;
		
	UDR1 = data;
}

unsigned char UART1_receive(void)
{
	while ( !(UCSR1A & (1<<RXC1)) )
		;
		
	return UDR1;
}

void UART1_print_string(unsigned char *str)
{
	for (int i=0; str[i]; i++)
		UART1_transmit(str[i]);
}

uint8_t UART1_IsRxD()
{
	return (UCSR1A & (1<<7));
}

uint8_t UART1_IsRxString()
{
	return rx1ReadyFlag;
}

uint8_t* UART1_GetRxString()
{
	rx1ReadyFlag = 0;
	
	return rx1String;
}

void UART1_ISR_Receive()
{
	volatile static uint8_t head=0;
	volatile uint8_t data;
	
	data = UDR1;
	// UART0_transmit(data);
	if (data == '\n' || data == '\r')
	{
		if (head) rx1String[head] = '\0';
		head=0;
		rx1ReadyFlag = 1;
	}
	else
	{
		rx1String[head] = data;
		head++;
	}
}
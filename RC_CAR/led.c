/*
 * led.c
 *
 * Created: 2022-08-09 오후 2:39:40
 *  Author: kccistc
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "led.h"

int led_command = 0;  // nothing

void init_led()
{
	AUTO_RUN_LED_PORT_DDR |= 1 << AUTO_RUN_LED_PIN;
}

int toggle = 0;
void ledalltoggle()
{
	toggle = !toggle;
	if(toggle)
		PORTA = 0xff;
	else
		PORTA = 0x00;
	_delay_ms(300);
}

void shift_left_led_on(void)
{
	for (int i = 0; i<8; i++)
	{
		PORTA = 0b00000001 << i;
		_delay_ms(300);
	}
}

void shift_right_led_on(void)
{
	for (int i = 0; i<8; i++)
	{
		PORTA = 0b10000000 >> i;
		_delay_ms(300);
	}
}

void shift_stack_left_led_on(void)
{
	PORTA = 0x00;
	for (int i = 0; i<8; i++)
	{
		PORTA += 0b00000001 << i;
		_delay_ms(300);
	}
}

void shift_stack_right_led_on(void)
{
	PORTA = 0x00;
	for (int i = 0; i<8; i++)
	{
		PORTA += 0b10000000 >> i;
		_delay_ms(300);
	}
}
void shift_flower_left_led_on(void)
{
	for (int i = 0; i<4; i++)
	{
		PORTA = (0b00000001 << i) + (0b10000000 >> i);
		_delay_ms(300);
	}
}

void shift_flower_right_led_on(void)
{
	for (int i = 0; i<4; i++)
	{
		PORTA = (0b00010000 << i) + (0b00001000 >> i);
		_delay_ms(300);
	}
}
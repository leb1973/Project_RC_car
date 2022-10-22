/*
 * button.h
 *
 * Created: 2022-08-04 오후 4:08:47
 *  Author: kcci
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LED_DDR		DDRA    // 이렇게 하는 이유는 LED_DDR에서 바꿔주면 COMPILER가 알아서 변경
#define LED_PORT    PORTA
#define BUTTON_DDR	DDRC
#define BUTTON_PIN  PINC  // PortC를 read 하는 register
// PINx는 하나의 메모리 AVR에서 제공하는 register
// 전압 5v를 읽으면 1, Ov 를 읽으면 0
#define BUTTON1		7    // PORT-C-7



#endif /* BUTTON_H_ */
/*
 * ultrasonic.h
 *
 * Created: 2022-08-10 오후 4:24:05
 *  Author: kccistc
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define FALSE 0
#define TRUE 1

#define US_TCNT_LEFT TCNT3	// timer3 16bit
#define TRIG_LEFT 0
#define TRIG_DDR_LEFT DDRA	// TRIGGER PIN
#define TRIG_PORT_LEFT PORTA
// HW external INT 4 : PE4
#define ECHO_LEFT 4
#define ECHO_DDR_LEFT DDRE
#define ECHO_PIN_LEFT PINE

#define US_TCNT_CENTER TCNT3	// timer3 16bit
#define TRIG_CENTER 1
#define TRIG_DDR_CENTER DDRA	// TRIGGER PIN
#define TRIG_PORT_CENTER PORTA
// HW external INT 4 : PE4
#define ECHO_CENTER 5
#define ECHO_DDR_CENTER DDRE
#define ECHO_PIN_CENTER PINE

#define US_TCNT_RIGHT TCNT3	// timer3 16bit
#define TRIG_RIGHT 2
#define TRIG_DDR_RIGHT DDRA	// TRIGGER PIN
#define TRIG_PORT_RIGHT PORTA
// HW external INT 4 : PE4
#define ECHO_RIGHT 6
#define ECHO_DDR_RIGHT DDRE
#define ECHO_PIN_RIGHT PINE

void init_ultrasonic();
void ultrasonic_trigger();

volatile int ultrasonic_left_distance;
volatile int ultrasonic_center_distance;
volatile int ultrasonic_right_distance;

#endif /* ULTRASONIC_H_ */
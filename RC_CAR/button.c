/*
 * button.c
 *
 * Created: 2022-08-04 오후 4:12:15
 *  Author: kcci
 */ 
#include "button.h"
#include "led.h"

int get_button1();

int button1_state=0;

void auto_mode_check()
{
	if (get_button1())
	{
		button1_state = !button1_state;
	}
	if (button1_state)
	{
		AUTO_RUN_LED_PORT |= 1 << 4;	// LED on
	}
	else
	{
		AUTO_RUN_LED_PORT &= ~(1 << 4);	// LED off
		stop();
	}
}

void init_button()
{
	
	BUTTON_DDR &=  ~((1 << BUTTON1 ));   // input : 0으로 설정
}

int get_button1()
{
	static uint8_t prev_state=0;  // 버튼의 상태 0: off 1: on
	uint8_t current_state;  // unsigned char current_state
	
	current_state = BUTTON_PIN & (1 << BUTTON1);  // button1값을 읽는다. 0x10
	if (prev_state == 0 && current_state !=0)  // 버튼이 처음 눌려진 상태 
	{
		_delay_ms(60);   // 노이즈가 지나가기를 기다린다.
		prev_state=1;    // 처음 눌려진 상태 가 아니다. 
		return 0;        // 아직은 진짜로 눌려진 상태가 아니다. 
	}
	else if (prev_state == 1 && current_state == 0) // 버튼이 눌렸다prev_state == 1 떠이진 상태 current_state == 0 
	{
		prev_state=0;
		_delay_ms(30);  // 노이즈가 지나가기를 기다린다.
		return 1;    // 완전히 눌렸다 떼어진 상태로 인정 
	}
	return 0;  // 아직 완전히 S/W를 눌렀다  떼지않은 상태 
}

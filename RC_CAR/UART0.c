/*
 * UART0.c
 *
 * Created: 2022-08-09 오전 10:45:52
 *  Author: kccistc
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>  // 
#include "UART0.h"

ISR(USART0_RX_vect)
{
   unsigned char data;
   
   data = UDR0;							//UDR0 = UART0의  하드웨어 레지스터로 1byte를 읽어 들인다.
   if(data == '\r' || data == '\n')		//문장의 끝이면
   {
      rx_buff[i] = '\0';				//문장의 끝을 가리키는 NULL
      i=0;								//개선점: circular queue (환형큐)로 개선을 해야한다.
										//이렇게 짜면 새로운 메세지가 오면 덮어쓴다.
	  rx_ready_flag = 1;
   }
   else
   {
      rx_buff[i++] = data;				// 1.rx_buff[i] = data 2. i++
   }
}

//1.전송속도 설정9600bps 총byte 글자수 :9600/10 ==> 960자 1글자 송수신 소요시간 약 1ms
//2.비동기식, data = 8 bit , none parity
//3. RX(수신) interrupt 활성화 // 데이터를 놓치지 않기 위함
//****TX 폴링 while 안에서 함수 호출**

void init_uart0(void)
{
   UBRR0H = 0x00;
   UBRR0L = 207;						//9600bps P219표 9-9
   
   UCSR0A |= (1 << U2X0);				// 2배속 통신
   UCSR0C |= 0x06;						// ASYNC 비동기 통신 data 8bit, none parity

//RXEN0 URAT0로부터 수신이 가능하도록 설정
//TXEN0 URAT0로부터 송신이 가능하도록 설정
// RXCIE0 :URAT0로 부터 1byte가 들어오면 RX (수신) interrupt를 띄어 달라

   UCSR0B = (1<< RXEN0) |(1 << TXEN0)|(1 << RXCIE0); 
}

//uart0로 1 byte를 보내는 함수
void UART0_transmit(uint8_t data )
{
   while (!(UCSR0A & (1 << UDRE0)))				//data가 전송중이면data가 다 전송 될때까지 기다린다.
   ;											//no operation 아무런 action도 취하지 않는다.
   
   UDR0 =data;									//HW 전송 register에 data를 쏴준다.
}
//ledalltoggle : 300ms 주기로 led전체를 on/0ff 반복한다.
extern int led_command;
void pc_command_processing()
{
	if(rx_ready_flag)  // if(rx_ready_flag >= 1)이랑 똑같음
	{
		rx_ready_flag = 0;
		printf("%s\n",rx_buff);
		if(strncmp(rx_buff, "ledallon", strlen("ledallon") - 1) == 0)
			PORTA = 0xff;
		if(strncmp(rx_buff, "ledalloff", strlen("ledalloff") - 1) == 0)
			PORTA = 0x00;
		if(strncmp(rx_buff, "reset", strlen("reset") - 1) == 0)
			led_command = 0;
		if(strncmp(rx_buff, "ledalltoggle", strlen("ledalltoggle") - 1) == 0)
			led_command = 1;
		if(strncmp(rx_buff, "shift_left_ledon", strlen("shift_left_ledon") - 1) == 0)
		led_command = 2;
		if(strncmp(rx_buff, "shift_right_ledon", strlen("shift_right_ledon") - 1) == 0)
		led_command = 3;
		if(strncmp(rx_buff, "shift_stack_left_ledon", strlen("shift_stack_left_ledon") - 1) == 0)
		led_command = 4;
		if(strncmp(rx_buff, "shift_stack_right_ledon", strlen("shift_stack_right_ledon") - 1) == 0)
		led_command = 5;
		if(strncmp(rx_buff, "shift_flower_left_ledon", strlen("shift_flower_left_ledon") - 1) == 0)
		led_command = 6;	
		if(strncmp(rx_buff, "shift_flower_right_ledon", strlen("shift_flower_right_ledon") - 1) == 0)
		led_command = 7;									
	}
		switch(led_command)
		{
			case 1:
			ledalltoggle();  // 우리보고 만들으라고????? 말 다했나 안되겠다 한마디 해야지
							// 교수님~ 쉬는 시간 쉽니까?
			break;
			case 2:
			shift_left_led_on();
			break;
			case 3:
			shift_right_led_on();
			break;
			case 4:
			shift_stack_left_led_on();
			break;
			case 5:
			shift_stack_right_led_on();
			break;
			case 6:
			shift_flower_left_led_on();
			break;
			case 7:
			shift_flower_right_led_on();
			break;
		}
}
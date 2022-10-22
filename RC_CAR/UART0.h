/*
 * UART0.h
 *
 * Created: 2022-08-09 오전 10:45:37
 *  Author: kccistc
 */ 


#ifndef UART0_H_
#define UART0_H_
void init_uart0(void);
void UART0_transmit(uint8_t data);
void pc_command_processing();

//UART0로 부터 1byte가 들어오면 rx(수신)interrupt 발생 되어 이곳으로 들어온다.
//P278 표 12-3참조
volatile unsigned char rx_buff[100]; //최적화 방지 volatile
volatile uint8_t rx_ready_flag = 0;
volatile int i = 0;


#endif /* UART0_H_ */
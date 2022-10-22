/*
 * ultrasonic.c
 *
 * Created: 2022-08-10 오후 4:24:14
 *  Author: kccistc
 */ 

#include "ultrasonic.h"

// 왼쪽 초음파 센서 PE4 : 외부 INT4 초음파 센서 상승, 하강 엣지 둘다 이쪽으로 들어온다.
// 초음파 : 1cm 를 이동하는데 29us 소요 -> 왕복 시간 : 29*2 = 58us
// 16000000Hz / 1024 ==> 15,625Hz
// T = 1/f ==> 0.0000064sec ==> 6.4us
ISR(INT4_vect)
{
	if (ECHO_PIN_LEFT & (1 << ECHO_LEFT))		// 상승 엣지
	{
		TCNT3=0;
	}
	else  // 하강 엣지
	{
		// echo pin 의 펄스 길이를 us 단위로 환산
		ultrasonic_left_distance = (1000000.0 * TCNT3 * 1024 / F_CPU) / 58;
		// 1cm : 58us 소요
	}
}

// 외부 INT 5번 : 중앙 초음파 센서
ISR(INT5_vect)
{
	if (ECHO_PIN_CENTER & (1 << ECHO_CENTER))		// 상승 엣지
	{
		TCNT3=0;
	}
	else  // 하강 엣지
	{
		// echo pin 의 펄스 길이를 us 단위로 환산
		ultrasonic_center_distance = (1000000.0 * TCNT3 * 1024 / F_CPU) / 58;
		// 1cm : 58us 소요
	}
}

// 외부 INT 6번 : 오른쪽 초음파 센서
ISR(INT6_vect)
{
	if (ECHO_PIN_RIGHT & (1 << ECHO_RIGHT))		// 상승 엣지
	{
		TCNT3=0;
	}
	else  // 하강 엣지
	{
		// echo pin 의 펄스 길이를 us 단위로 환산
		ultrasonic_right_distance = (1000000.0 * TCNT3 * 1024 / F_CPU) / 58;
		// 1cm : 58us 소요
	}
}


// 초음파 센서 초기화 함수
void init_ultrasonic()
{
	//=============== LEFT ultrasonic ===============
	TRIG_DDR_LEFT |= (1 << TRIG_LEFT);	// output mode 로 설정
	ECHO_DDR_LEFT &=	~(1 << ECHO_LEFT);	// input  mode 로 설정
	
	// EICRA : INT 0~3
	// EICRB : INT 4~7
	// 0 1   : 어떠한 신호의 변화가 있든지 INT를 요청한다. (상승, 하강 엣지 둘 다 INT 발생)
	EICRB |= (0 << ISC41) | (1 << ISC40);
	TCCR3B |= (1 << CS32) | (1 << CS30);	// 1024 로 분주
	// 16bit timer 에서 1번을 1024 분주 해서 공급한다.
	// 16MHz 를 1024 로 분주 --> 16000000/1024 ==> 15625Hz ==> 15.625KHz
	// 1펄스의 길이 : t = 1/f --> 1/15625 = 0.000064sec ==> 64us
	EIMSK |= (1 << INT4);	// 외부 interrupt 4번 INT4(ECHO) 사용
	
	//=============== CENTER ultrasonic ===============
	TRIG_DDR_CENTER |= (1 << TRIG_CENTER);	// output mode 로 설정
	ECHO_DDR_CENTER &=	~(1 << ECHO_CENTER);	// input  mode 로 설정
	
	// EICRA : INT 0~3
	// EICRB : INT 4~7
	// 0 1   : 어떠한 신호의 변화가 있든지 INT를 요청한다. (상승, 하강 엣지 둘 다 INT 발생)
	EICRB |= (0 << ISC51) | (1 << ISC50);
	TCCR3B |= (1 << CS32) | (1 << CS30);	// 1024 로 분주
	// 16bit timer 에서 1번을 1024 분주 해서 공급한다.
	// 16MHz 를 1024 로 분주 --> 16000000/1024 ==> 15625Hz ==> 15.625KHz
	// 1펄스의 길이 : t = 1/f --> 1/15625 = 0.000064sec ==> 64us
	EIMSK |= (1 << INT5);	// 외부 interrupt 4번 INT4(ECHO) 사용
	
	//=============== RIGHT ultrasonic ===============
	TRIG_DDR_RIGHT |= (1 << TRIG_RIGHT);	// output mode 로 설정
	ECHO_DDR_RIGHT &=	~(1 << ECHO_RIGHT);	// input  mode 로 설정
	
	// EICRA : INT 0~3
	// EICRB : INT 4~7
	// 0 1   : 어떠한 신호의 변화가 있든지 INT를 요청한다. (상승, 하강 엣지 둘 다 INT 발생)
	EICRB |= (0 << ISC61) | (1 << ISC60);
	TCCR3B |= (1 << CS32) | (1 << CS30);	// 1024 로 분주
	// 16bit timer 에서 1번을 1024 분주 해서 공급한다.
	// 16MHz 를 1024 로 분주 --> 16000000/1024 ==> 15625Hz ==> 15.625KHz
	// 1펄스의 길이 : t = 1/f --> 1/15625 = 0.000064sec ==> 64us
	EIMSK |= (1 << INT6);	// 외부 interrupt 4번 INT4(ECHO) 사용
}

void distance_check(void)
{
	ultrasonic_trigger();
	
	printf("left: %5dcm\n", ultrasonic_left_distance);
	printf("center: %5dcm\n", ultrasonic_center_distance);
	printf("right: %5dcm\n", ultrasonic_right_distance);
}

void ultrasonic_trigger()
{
	//=============== LEFT ultrasonic ===============
	TRIG_PORT_LEFT &= ~(1 << TRIG_LEFT);	// LOW
	_delay_us(1);
	TRIG_PORT_LEFT |= (1 << TRIG_LEFT);		// 1: HIGH
	_delay_us(15);
	TRIG_PORT_LEFT &= ~(1 << TRIG_LEFT);	// LOW
	
	_delay_ms(50);		// 초음파 센서 echo 응답 시간 최대 38ms
						// _delay_ms 를 timer INT 로 처리하나. delay 가 있어서는 안된다.
						// 현재는 demo code 지만 여러분은 timer 로 처리해야한다.
						
	//=============== CENTER ultrasonic ===============
	TRIG_PORT_CENTER &= ~(1 << TRIG_CENTER);	// LOW
	_delay_us(1);
	TRIG_PORT_CENTER |= (1 << TRIG_CENTER);		// 1: HIGH
	_delay_us(15);
	TRIG_PORT_CENTER &= ~(1 << TRIG_CENTER);	// LOW
	
	_delay_ms(50);		// 초음파 센서 echo 응답 시간 최대 38ms
						// _delay_ms 를 timer INT 로 처리하나. delay 가 있어서는 안된다.
						// 현재는 demo code 지만 여러분은 timer 로 처리해야한다.
	
	//=============== RIGHT ultrasonic ===============
	TRIG_PORT_RIGHT &= ~(1 << TRIG_RIGHT);	// LOW
	_delay_us(1);
	TRIG_PORT_RIGHT |= (1 << TRIG_RIGHT);		// 1: HIGH
	_delay_us(15);
	TRIG_PORT_RIGHT &= ~(1 << TRIG_RIGHT);	// LOW
	
	_delay_ms(50);		// 초음파 센서 echo 응답 시간 최대 38ms
						// _delay_ms 를 timer INT 로 처리하나. delay 가 있어서는 안된다.
						// 현재는 demo code 지만 여러분은 timer 로 처리해야한다.
}
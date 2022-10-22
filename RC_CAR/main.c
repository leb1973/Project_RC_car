/*
 * RC_CAR.c
 * Created: 2022-08-05 오전 11:46:29
 * Author : kcci
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h> 
#include <avr/interrupt.h>
#include <stdio.h> // printf , scanf 등이 정의 되어있다.
#include <string.h>//strcpy,strncmp,strcat 등 이 들어있다.


extern void UART0_transmit(uint8_t data);
extern void init_UART1();
extern void distance_check();
extern void init_ultrasonic();
extern void init_led();
extern int button1_state;
extern int ultrasonic_left_distance;
extern int ultrasonic_center_distance;
extern int ultrasonic_right_distance;

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL , _FDEV_SETUP_WRITE);
uint32_t ms_count=0;  // ms를 재는 변수
uint32_t sec_count=0;  // sec를 재는 변수
 

// PORTB.0 : 500ms  on/off
// PORTB.1 : 300ms on/off
// 1. 분주: 64분주 ==> 16000000/64 ==> 250,000HZ
// 2. T 계산 ==> 1/f = 1/250000 = > 0.000004 sec (4us) : 0.004ms
// 3. 8 bit Timer OV: 0.004ms x 256 ==> 0.00124sec ==> 약 1.24ms
//                    0.004ms x 250 ==> 1ms
// 256개의 pulse를 count를 하면 이곳으로 온다.
// 1ms 마다 ISR(TIMER0_OVF_vect) 이곳으로 들어 온다
ISR(TIMER0_OVF_vect)
{
   TCNT0 = 6;
   
   ms_count++;
   if (ms_count >= 1000)   // 1000ms ==> 1sec
   {
      ms_count=0;
      sec_count++;    // sec counter증가
   }
}
#define MOTOR_DRIVER_PORT       PORTF
#define MOTOR_DRIVER_PORT_DDR   DDRF

#define MOTOR_DDR               DDRB
#define MOTOR_RIGHT_PORT_DDR    5  // OC1A
#define MOTOR_LEFT_PORT_DDR     6  // OC1B

int main(void)
{ 
   init_led();
   init_button();
   init_uart0(); //uart0를 초기화
   init_UART1(); // uart1 초기화
   init_timer0();  
   init_pwm_motor();    
   init_ultrasonic();
   stdout = &OUTPUT;
   sei();                     // 전역적으로 인터럽트 허용


    while (1) 
    {
		manual_mode_run ();		// Blue-tooth car command run
		distance_check();		// 초음파 거리 측정
		auto_mode_check();		// button 1 check
		
		if (button1_state)
		{
			auto_drive();
		}
		
    }
}


/* 자율주행 프로그램을 이곳에 programing 한다. 
   printf("auto_drive !!! \n");
   초음파 센서 앞에 거리가 얼마냐~ 
   ultrasonic_left_distance >> 5 이런식으로 
   튀는 초음파 값을 잡는다 
*/

void auto_drive(void)
{
	
	printf("auto drive !!!\n");
	 forward(650);
	 
	 if(ultrasonic_left_distance < 30)
	 {
		 turn_right(600);
	 }
	
	 if(ultrasonic_center_distance < 30)
	 {
		 stop();
		
		  if(ultrasonic_right_distance < ultrasonic_left_distance )
		  {
			  turn_left(600);
		  }
		  if(ultrasonic_right_distance > ultrasonic_left_distance )
		  {
			  turn_right(600);
		  }
	 }
	 
	 if(ultrasonic_right_distance <30)
	 {
		 turn_left(600);
	 }
	 
	 if(ultrasonic_center_distance <= 3)
	 {
		 backward(500);
		 if(ultrasonic_right_distance < ultrasonic_left_distance )
		 {
			 turn_left(600);
		 }
		 if(ultrasonic_right_distance > ultrasonic_left_distance )
		 {
			 turn_right(600);
		 }
	 }
		
		
	
}

extern volatile unsigned char UART1_RX_data;

void manual_mode_run (void)
{
	switch(UART1_RX_data)
	{
		case 'F':
		forward(1200);  // 4us * 500 = 0.002 (2ms)
		break;
		case 'B':
		backward(700);
		break;
		case 'L':
		turn_left(800); 
		break;
		case 'R':
		turn_right(800);
		break;
		case 'S':
		stop();
		break;
		default:
		break;
	}
}

void forward(int speed)
{
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));  // 전진,후진 제어
	MOTOR_DRIVER_PORT |= (1 << 2) | (1 << 0);  // 자동차를 전진 모드로 setting
	
	OCR1A = speed;  // PB5 PWM 출력, 왼쪽 바퀴
	OCR1B = speed;  // PB5 PWM 출력, 오른쪽 바퀴 
}

void backward(int speed)
{
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));  // 전진,후진 제어
	MOTOR_DRIVER_PORT |= (1 << 3) | (1 << 1);  // 자동차를 후진 모드로 setting (0101)
	
	OCR1A = speed;  // PB5 PWM 출력, 왼쪽 바퀴
	OCR1B = speed;  // PB5 PWM 출력, 오른쪽 바퀴
}

void turn_left(int speed)
{
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));  // 전진,후진 제어
	MOTOR_DRIVER_PORT |= (1 << 2) | (1 << 0);  // 자동차를 전진 모드로 setting
	
	OCR1A = 0;              // PB5 PWM 출력, 왼쪽 바퀴
	OCR1B = speed;          // PB5 PWM 출력, 오른쪽 바퀴	
}

void turn_right(int speed)
{
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));  // 전진,후진 제어
	MOTOR_DRIVER_PORT |= (1 << 2) | (1 << 0);  // 자동차를 전진 모드로 setting
	
	OCR1A = speed;              // PB5 PWM 출력, 왼쪽 바퀴
	OCR1B = 0;                  // PB5 PWM 출력, 오른쪽 바퀴
}

void stop()
{
		
		OCR1A = 0;              // PB5 PWM 출력, 왼쪽 바퀴
		OCR1B = 0;                  // PB5 PWM 출력, 오른쪽 바퀴
		
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));  // 전진,후진 제어
	MOTOR_DRIVER_PORT |= (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0);     // 자동차를 stop 모드로 setting

}
/*
	1. left motor
	PORTF.0 = IN1
	PORTF.1 = IN2
	2. right motor
	PORTF.2 = IN3
	PORTF.3 = IN4
	IN1.IN3 IN2.IN4
	======= =======
	   0       1   : 역회전 
	   1       0   : 정회전 
	   1       1   : STOP    // PORTF 를 제어함 
*/
 

void init_pwm_motor(void)
{
	MOTOR_DRIVER_PORT_DDR |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);     // Motor Drive port
	MOTOR_DDR |= (1 << MOTOR_RIGHT_PORT_DDR) | (1 << MOTOR_LEFT_PORT_DDR);  // PWM port
	
	TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);  // 64분주 
														// 16.000.000Hz / 64 = 250.000Hz (timer 1번에 공급되는 clock)
														// 256 / 250.000 = 1.02ms (pulse를 256개를 count하면 1.02ms 소요)
														// 127 / 250.000 = 0.5ms
														// 0x3ff(1023) = 4ms - Motor 최대 속도 
	TCCR1B |= (1 << WGM13) | (1 << WGM12);  // 모드 14 고속 PWM ICR1
	TCCR1A |= (1 << WGM11) | (0 << WGM10);  // 모드 14 고속 PWM ICR1
	TCCR1A |= (1 << COM1A1) | (0 << COM1A0); // 비반전 모드 : OCR: LOW ,TOP: HIGH
	TCCR1A |= (1 << COM1B1) | (0 << COM1B0); // 비반전 모드 : OCR: LOW ,TOP: HIGH
	
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));  // 전진,후진 제어 
	MOTOR_DRIVER_PORT |= (1 << 2) | (1 << 0);  // 자동차를 전지 모드로 setting
	ICR1 = 0X3ff;  // 1023 = 4ms
}

void init_timer0()
{
   TCNT0=6;   // TCNT 6~256 ==> 정확히 1ms 마다 TIMT 0 OVF INT 
   // 분주비를 64로 설정 P269 표13-1
   TCCR0 |= (1 << CS02) | (0 << CS01) | (0 << CS00);

   TIMSK |= (1 << TOIE0);         // 오버플로 인터럽트 허용
   
}
/*
 * Project.c
 *
 *
 */

#include <xc.h>
//#include <stdio.h>
#include <delay.h>
// Voltage Reference: AREF pin
#define ADC_VREF_TYPE ((0<<REFS1) | (0<<REFS0) | (0<<ADLAR))

int count = 0;  //  초를 세기 위한 카운터
int count2 = 0;
int h = 0, m = 0, s = 0;   //  현재시간 저장 변수
int ah = 0, am = 0; // 알람세팅 변수
int alarm_enable = 0;  // 알람 수행 enable
int timer_alarm_enable = 0;   //  타이머 알람 수행 enable
char data[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x67};  //  세그먼트 숫자
char point_data[10] = {0xbf, 0x86, 0xdb, 0xcf, 0xe6, 0xed, 0xfd, 0xa7, 0xff, 0xe7}; // "숫자."
int segment_enable = 0;  //  세그먼트 실행 enable
int digit = 0;            // 세그먼트 값 지정을 위한 변수
int num1 = 0, num10 = 0, num100 = 0, num1000 = 0;     // 세그먼트 숫자 출력을 위한 변수
int tm = 0, ts = 0;  // 타이머 세팅 변수 1
int t1 = 0, t10 = 0, t100 = 0, t1000 = 0;  //  타이머 세팅 변수 2
float temp;  //  가변저항으로 값 받기위한 변수
int alarm_power;  //  가변저항으로 데이터를 받아서 저장한 알람의 세기값
int power_enable = 0;  // 알람파워를 설정하는 enable
int alarm_setting_enable = 0, timer_setting_enable = 0;  //  알람, 타이머 세팅 enable
int light = 0;    //  현재 백라이트 상태를 알려주는 변수

// ADC 설정
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | ADC_VREF_TYPE;
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=(1<<ADSC);
// Wait for the AD conversion to complete
while ((ADCSRA & (1<<ADIF))==0);
ADCSRA|=(1<<ADIF);
return ADCW;
}

// 설정된 알람시간이 되면 출력 발생
void alarm_output() {
    printf("$C\r");
    printf("$G, 1, 1\r");
    printf("$T,Alarm output!\r");
    while(1){        //  알람이 계속 울리고 버튼 4를 누르면 꺼진다.
        if(alarm_power == 0) {  // PWM 60%
            PORTE = 0b00001000;     //  포트 E에서 모터제어를 하고 2번모터를 사용
            delay_us(60);
            PORTE = 0b00000000;
            delay_us(40);    
        }
        else if( alarm_power == 1) {   // PWM 70%
            PORTE = 0b00001000;
            delay_us(70);
            PORTE = 0b00000000;
            delay_us(30);
        }
        else if( alarm_power == 2) {  // PWM 80%
            PORTE = 0b00001000;
            delay_us(80);
            PORTE = 0b00000000;
            delay_us(20);
        }    
        if(PIND.4 == 1) {
            PORTB = 0b00010000;
            delay_ms(100);
            break;}
    }    
    printf("$C\r");    
}

// 타이머의 세팅 시간을 세그먼트에 적용하기 위한 함수
void setting_timernum(int a, int b){
    t1 = b % 10;
    t10 = b / 10;
    t100 = a % 10;
    t1000 = a / 10; 
    num1 = t1, num10 = t10, num100 = t100, num1000 = t1000;       
}

//  알람 세기를 LCD에 출력하는 함수
void power_print(){
    printf("$C\r");
    printf("$G, 1, 1\r");
    printf("$T,  Alarm Power  \r"); 
    printf("$G,2,1\r");
    printf("$T,  volume : %d    \r", alarm_power + 1);
    delay_ms(1500);
    printf("$C\r");    
}

//  알람 세팅 함수
void alarm_setting_function() {
    // 버튼 4,5 -> 분(m)시(h) 설정 , 버튼 6 -> 알람세팅완료
    // , 버튼 7 -> 알람세팅취소(alarm_enable = 0)  
    // 각 버튼을 누를 때 해당 비트의 LED가 켜진다.
    while(1){
        if(PIND.7 == 1) { 
            alarm_enable = 0; 
            PORTB = 0b10000000;
            break; }
        else if(PIND.4 == 1) { 
            am++;   
            PORTB = 0b00010000;
            delay_ms(100); }
        else if(PIND.5 == 1) { 
            ah++;  
            PORTB = 0b00100000;
            delay_ms(100); }
        else if(PIND.6 == 1) { 
            alarm_enable = 1;
            PORTB = 0b01000000;
            break; }
        if(am == 60) {am = 0;}
        if(ah == 24) {ah = 0;}
        
        printf("$G, 1, 1\r");  //  현재 설정되는 알람시간
        printf("$T, Alarm Setting  \r");
        printf("$G, 2, 1\r"); 
        printf("$T, %02d : %02d : 00   \r", ah,am);
    }  
    printf("$C\r");
}

//  타이머 세팅 함수
void timer_setting_function() {
// 버튼 4,5:타이머세팅, 버튼 6:타이머적용 및 이어하기, 버튼 7:타이머취소 및 초기화    
    // 각 버튼을 누를 때 해당 비트의 LED가 켜진다.    
    if (segment_enable == 0){    //segment_enable == 0
    while(1) {                   //   타이머가 작동하지 않은 상태
        printf("$G, 1, 1\r");
        printf("$T,Timer Max(99m)  \r");
        printf("$G, 2, 1\r"); 
        printf("$T,  %02d m : %02d s   \r", tm,ts);
        if(PIND.4 == 1) {  
            PORTB = 0b00010000;
            ts++;
            delay_ms(100); }
        else if(PIND.5 == 1) { 
            tm++;   
            PORTB = 0b00100000;
            delay_ms(100); }
        else if(PIND.6 == 1) {   
            PORTB = 0b01000000;
            segment_enable = 1;   // 세그먼트 작동 변수
            setting_timernum(tm, ts); 
            break; }
        else if(PIND.7 == 1) { 
            PORTB = 0b10000000;
            segment_enable = 0;
            num1 = 0, num10 = 0, num100 = 0, num1000 = 0;
            ts = 0, tm = 0;
            break; }
        if(ts == 60) {ts = 0;}
        if(tm == 99) {tm = 0;}  
    }                       
    }    
    else if (segment_enable == 1){    
        segment_enable = 0;  //   segment_enable == 1 일때,
        while(1) {            //   0으로 만들어서 잠시 일시정지
        printf("$G, 1, 1\r");
        printf("$T,Timer Max(99m)  \r");
        printf("$G, 2, 1\r"); 
        printf("$T,  %02d m : %02d s   \r", tm,ts);
        if(PIND.4 == 1) {  
            PORTB = 0b00010000;
            ts++;
            delay_ms(100); }
        else if(PIND.5 == 1) { 
            tm++;   
            PORTB = 0b00100000;
            delay_ms(100); }
        else if(PIND.6 == 1) {  //  기존의 타이머를 이어한다.
            PORTB = 0b01000000;
            segment_enable = 1;
            setting_timernum(tm, ts); 
            break; }
        else if(PIND.7 == 1) { 
            PORTB = 0b10000000;
            segment_enable = 0;
            num1 = 0;
            num10 = 0;
            num100 = 0;
            num1000 = 0;
            ts = 0, tm = 0;
            break; }
        if(ts == 60) {ts = 0;}
        if(tm == 99) {tm = 0;}  
    }    
    }
    printf("$C\r");
    printf("$B,0\r");
}
               
// Timer0 을 이용해서 흘러가는 시간 구현
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
    // 1 ms
    TCNT0=0x06;
    count++;
    
    if(count == 1000){
        count = 0;
        s++;           
        if(s == 60){
            s = 0;
            m++;
            if(m == 60){
                m = 0;
                h++;
                if(h == 24){
                    h = 0;
                }
            }
        }
    }   
}

// Timer2 : 세그먼트를 이용한 타이머 설정
interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
    TCNT2=0x06; 
    count2++;
    if(count2 == 1000){
        count2 = 0;     //  타이머값이 주어지면 초마다 1씩 감소한다.
        if( segment_enable == 1){
            num1--;
            if(num1 == -1) {
                num1 = 9;
                num10--;
                if(num10 == -1){
                    num10 = 5;
                    num100--;
                    if(num100 == -1){
                        num100 = 9;
                        num1000--;
                        if(num1000 == -1){
                            num1000 = 9;
                        }
                    }
                }
            }       //  ts, tm에 현재 세그먼트 값을 넣어서 일시정지 할 수 있는 방법을 확보함.   
            ts = num10 * 10 + num1;
            tm = num1000 * 10 + num100;   
                        //  '00분00초'가 되면 timer_alarm_enable 이 1이 되어 알람이 출력.
            if(num1 == 0 && num10 == 0 && num100 == 0 && num1000 == 0){ 
                printf("$C\r"); 
                timer_alarm_enable = 1;
                segment_enable = 0;//segment_enable 를 0으로 만들어서 타이머감소가 없게 한다.
                ts = 0, tm = 0;    
            }
        }
    }
    PORTC = ~(1<<digit);
    if(digit == 0){ PORTA = data[num1000]; }
    else if( digit == 1) { PORTA = point_data[num100]; }   
     //  세번째 자리 세그먼트의 숫자에 '.'을 표현하여 
     //   분(m)과 초(s)를 구분하였다. 
    else if( digit == 2) { PORTA = data[num10]; }           
    else { PORTA = data[num1]; }    
    delay_us(10);
    digit++;
    digit = digit%4;
    
}

// 외부 인터럽트 0 -> 현재시간 세팅 인터럽트
interrupt [EXT_INT0] void ext_int0_isr(void)
{
    PORTB = 0b00000001;
    printf("$C\r");   
    // 버튼 4~6 각각 초(s)분(m)시(h) 세팅버튼. 버튼 7은 저장후 세팅모드에서 나가기  
    // 각 버튼을 누를 때 해당 비트의 LED가 켜진다.    
    while(1){
        if(PIND.4 == 1) { 
            s++;
            PORTB = 0b00010000;
            delay_ms(100); }
        else if(PIND.5 == 1) { 
            m++; 
            PORTB = 0b00100000;
            delay_ms(100); }
        else if(PIND.6 == 1) { 
            h++;   
            PORTB = 0b01000000;
            delay_ms(100); }
        else if(PIND.7 == 1) {
            PORTB = 0b10000000;
            break; }      
        if(s == 60) {s = 0;}
        if(m == 60) {m = 0;}
        if(h == 24) {h = 0;}       
        printf("$G, 1, 1\r");
        printf("$T,Setting, SW 4-7\r"); 
        printf("$G, 2, 1\r"); 
        printf("$T, %02d : %02d : %02d   \r", h,m,s);
    }  
    printf("$C\r");
}

// 외부 인터럽트 1 알람시계 세팅 인터럽트
interrupt [EXT_INT1] void ext_int1_isr(void)
{
    PORTB = 0b00000010;
    printf("$C\r");
    alarm_setting_enable = 1;     //  알람 시간 세팅 함수 수행을 위함.
       // 인터럽트에서 세팅하면 현재 시간이 멈추기 때문에 while문에서 
       //  alarm_setting_enable = 1  를 확인하면 함수를 콜한다.
}

// 외부 인터럽트 2 : 알람세기조절
interrupt [EXT_INT2] void ext_int2_isr(void)
{
    PORTB = 0b00000100;
    printf("$C\r"); 
    temp = read_adc(1);
    alarm_power = ((float)temp/1023)*(2); 
      //  가변저항에 따라서 1 ~ 3 단계로 세기를 지정함 
    power_enable = 1;
}

// 외부 인터럽트 3 : 세그먼트 타이머 설정
interrupt [EXT_INT3] void ext_int3_isr(void)
{
    PORTB = 0b00001000;
    printf("$C\r");  
    timer_setting_enable = 1;  
        // 인터럽트에서 세팅하면 현재 시간이 멈추기 때문에 while문에서 
       //  timer_setting_enable = 1  를 확인하면 함수를 콜한다.
}

void main(void)
{
    ASSR=0<<AS0;
    TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (1<<CS02) | (0<<CS01) | (0<<CS00);
    TCNT0=0x06;
    OCR0=0x00;  
    TCCR2=(0<<WGM20) | (0<<COM21) | (0<<COM20) | (0<<WGM21) | (0<<CS22) | (1<<CS21) | (1<<CS20);
    TCNT2=0x06;
    OCR2=0x00; 
    TIMSK=(0<<OCIE2) | (1<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (1<<TOIE0);
    ETIMSK=(0<<TICIE3) | (0<<OCIE3A) | (0<<OCIE3B) | (0<<TOIE3) | (0<<OCIE3C) | (0<<OCIE1C);
    // 타이머 설정    
    
    UCSR0A=(0<<RXC0) | (0<<TXC0) | (0<<UDRE0) | (0<<FE0) | (0<<DOR0) | (0<<UPE0) | (0<<U2X0) | (0<<MPCM0);
    UCSR0B=(0<<RXCIE0) | (0<<TXCIE0) | (0<<UDRIE0) | (0<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80);
    UCSR0C=(0<<UMSEL0) | (0<<UPM01) | (0<<UPM00) | (0<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00) | (0<<UCPOL0);
    UBRR0H=0x00;
    UBRR0L=0x67; 
    // UART0 설정     
    
    EICRA=(1<<ISC31) | (1<<ISC30) | (1<<ISC21) | (1<<ISC20) | (1<<ISC11) | (1<<ISC10) | (1<<ISC01) | (1<<ISC00);
    EICRB=(0<<ISC71) | (0<<ISC70) | (0<<ISC61) | (0<<ISC60) | (0<<ISC51) | (0<<ISC50) | (0<<ISC41) | (0<<ISC40);
    EIMSK=(0<<INT7) | (0<<INT6) | (0<<INT5) | (0<<INT4) | (1<<INT3) | (1<<INT2) | (1<<INT1) | (1<<INT0);
    EIFR=(0<<INTF7) | (0<<INTF6) | (0<<INTF5) | (0<<INTF4) | (1<<INTF3) | (1<<INTF2) | (1<<INTF1) | (1<<INTF0);       
    // 외부 인터럽트 설정             
    
    ADMUX=ADC_VREF_TYPE;
    ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADFR) | (0<<ADIF) | (0<<ADIE) | (1<<ADPS2) | (0<<ADPS1) | (1<<ADPS0);
    SFIOR=(0<<ACME);
    // ADC 설정
    
    #asm("sei")  
    
    DDRB = 0xff;
    PORTB = 0x00;
    // 포트 B : 스위치를 누르면 LED 출력            
    
    DDRA = 0xff;
    DDRC = 0xff;    
    PORTC = 0b00001111; 
    // 세그먼트 기본 세팅    
    
    DDRE = 0x0f;
    PORTE = 0x00;
    // DC 모터제어     
    
    DDRF=(0<<DDF7) | (0<<DDF6) | (0<<DDF5) | (0<<DDF4) | (0<<DDF3) | (0<<DDF2) | (0<<DDF1) | (0<<DDF0);
    PORTF=(0<<PORTF7) | (0<<PORTF6) | (0<<PORTF5) | (0<<PORTF4) | (0<<PORTF3) | (0<<PORTF2) | (0<<PORTF1) | (0<<PORTF0);
    //  ADC를 위한 포트 F 설정 
    
    printf("$I\r");
    printf("$C\r"); 
    printf("$L,0\r");
    printf("$B,0\r");
    // LCD 초기설정      
    
    temp = read_adc(1);
    alarm_power = ((float)temp/1023)*(5);  //  처음 시작할때 알람세기 초기 설정을 위함

while (1)
    {                      
    printf("$G, 1, 1\r");
    printf("$T,   Time         \r");
    printf("$G, 2, 1\r"); 
    printf("$T, %02d : %02d : %02d   \r", h,m,s);
    // 현재시각 출력  
    
    PORTB = 0x00;  //  일반적인 상태에서는 LED와 모터가 동작하지 않게 하기 위함.
    PORTE = 0x00;    
    
    if(alarm_setting_enable == 1){      //  알람시간 설정 함수 호출
        alarm_setting_function();
        alarm_setting_enable = 0;
    }       
    if(timer_setting_enable == 1) {    //  타이머 설정 함수 호출
        timer_setting_function();
        timer_setting_enable = 0;
    }
    if(h == ah && m == am && alarm_enable == 1){ //알람시계의 알람
        alarm_output();
        alarm_enable = 0;     
    } 
    if (timer_alarm_enable == 1){    //  타이머의 알람
        alarm_output();
        timer_alarm_enable = 0;    
    }          
    //  인터럽트로 인해서 가변저항의 값을 읽으면
    //  print 함수를 호출하여 알람의 세기를 보여준다. 
    if(power_enable == 1){ 
        power_print();
        power_enable = 0;
    }   
    if(PIND.5 == 1){     //  버튼 5를 누르면 백라이트 끄거나 켜기
        if (light == 0){
            printf("$L,1\r");
            light = 1;
        }
        else if (light == 1) {
            printf("$L,0\r");  
            light = 0;
        }    
    }
    
    }   
}

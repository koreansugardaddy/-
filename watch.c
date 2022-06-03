#include        <xc.h>
#include <delay.h>
#include    "DU MCU.h"
#include    "OLED_SSD1308Z.h"
#include    "Variable_Extern.h"
// Voltage Reference: AREF pin
#define ADC_VREF_TYPE ((0<<REFS1) | (0<<REFS0) | (0<<ADLAR))

int count = 0;  //  a counter for counting seconds 
int count2 = 0;
int h = 0, m = 0, s = 0;   //  Current time storage variable
int ah = 0, am = 0; // Alarm Setting Variables
int alarm_enable = 0;  // Performing an alarm enable
int timer_alarm_enable = 0;   //  Performing a timer alarm enable
char data[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x67};  //  Segment Numbers
char point_data[10] = {0xbf, 0x86, 0xdb, 0xcf, 0xe6, 0xed, 0xfd, 0xa7, 0xff, 0xe7}; // "Numbers."
int segment_enable = 0;  //  Run Segment enable
int digit = 0;            // Variables for specifying segment values
int num1 = 0, num10 = 0, num100 = 0, num1000 = 0;     // Variables for Segmented Numeric Output
int tm = 0, ts = 0;  // Timer Setting Variables 1 
int t1 = 0, t10 = 0, t100 = 0, t1000 = 0;  //  Timer Setting Variables 2 
float temp;  //  Variable to receive value with variable resistance
int alarm_power;  //  The value of the strength of the alarm that you received and stored data with a variable resistor 
int power_enable = 0;  // Set the alarm power enable
int alarm_setting_enable = 0, timer_setting_enable = 0;  //  Alarm, timer setting enable
int light = 0;    //  Variable that tells you the current backlight status

// ADC setting
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

// Output is generated when the alarm time is set
void alarm_output() {
    strcpy(sDisplay, "$C\r");
    Display_String(6, 0, 1, 0, sDisplay);
    strcpy(sDisplay, "$G, 1, 1\r");
    Display_String(6, 0, 1, 0, sDisplay);
    strcpy(sDisplay, "$T,Alarm output!\r");
    Display_String(6, 0, 1, 0, sDisplay);
    while(1){        //  The alarm keeps ringing and pressing button 4 turns off. 
        if(alarm_power == 0) {  // PWM 60%
            PORTE = 0b00001000;     //   Motor control at port E and motor number 2 is used
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
    strcpy(sDisplay, "$C\r");
    Display_String(6, 0, 1, 0, sDisplay);
    
}

// Function for applying the timer's setting time to the segment
void setting_timernum(int a, int b){
    t1 = b % 10;
    t10 = b / 10;
    t100 = a % 10;
    t1000 = a / 10; 
    num1 = t1, num10 = t10, num100 = t100, num1000 = t1000;       
}

//  Function to output alarm strength to LCD 
void power_print(){
    strcpy(sDisplay, "$C\r");
    Display_String(6, 0, 1, 0, sDisplay);
    strcpy(sDisplay, "$G, 1, 1\r");
    Display_String(6, 0, 1, 0, sDisplay);
    strcpy(sDisplay, "$T,  Alarm Power  \r");
    Display_String(6, 0, 1, 0, sDisplay);
    strcpy(sDisplay, "$G,2,1\r");
    Display_String(6, 0, 1, 0, sDisplay);
    strcpy(sDisplay, "$T,  volume : %d    \r", alarm_power + 1);
    Display_String(6, 0, 1, 0, sDisplay);
    delay_ms(1500);
    strcpy(sDisplay, "$C\r");    
    Display_String(6, 0, 1, 0, sDisplay);
}

//  Alarm setting function
void alarm_setting_function() {
    // button 4,5 -> (m),(h) setting , button 6 -> Alarm setting completed
    // , button  7 -> Cancel alarm setting(alarm_enable = 0)  
    // The LED of the corresponding bit is illuminated when each button is pressed.
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
        
        strcpy(sDisplay, "$G, 1, 1\r");  //  Current alarm time set 
        Display_String(6, 0, 1, 0, sDisplay);
        strcpy(sDisplay, "$T, Alarm Setting  \r");
        Display_String(6, 0, 1, 0, sDisplay);
        strcpy(sDisplay, "$G, 2, 1\r"); 
        Display_String(6, 0, 1, 0, sDisplay);
        strcpy(sDisplay, "$T, %02d : %02d : 00   \r", ah,am);
        Display_String(6, 0, 1, 0, sDisplay);
    }  
    strcpy(sDisplay, "$C\r");
    Display_String(6, 0, 1, 0, sDisplay);
}

//  Timer setting function 
void timer_setting_function() {
// button 4,5:timer setting, button 6:Applying and Continuing Timers, button 7:Cancel and timer reset.    
    // The LED of the corresponding bit is illuminated when each button is pressed.     
    if (segment_enable == 0){    //segment_enable == 0
    while(1) {                   //   Timer inoperative 
        strcpy(sDisplay, "$G, 1, 1\r");
        Display_String(6, 0, 1, 0, sDisplay);
        strcpy(sDisplay, "$T,Timer Max(99m)  \r");
        Display_String(6, 0, 1, 0, sDisplay);
        strcpy(sDisplay, "$G, 2, 1\r"); 
        Display_String(6, 0, 1, 0, sDisplay);
        strcpy(sDisplay, "$T,  %02d m : %02d s   \r", tm,ts);
        Display_String(6, 0, 1, 0, sDisplay);
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
            segment_enable = 1;   // Segment Operation Variables 
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
        segment_enable = 0;  //   segment_enable == 1, 
        while(1) {            //   Make it 0 and pause
        strcpy(sDisplay, "$G, 1, 1\r");
        Display_String(6, 0, 1, 0, sDisplay);
        strcpy(sDisplay, "$T,Timer Max(99m)  \r");
        Display_String(6, 0, 1, 0, sDisplay);
        strcpy(sDisplay, "$G, 2, 1\r"); 
        Display_String(6, 0, 1, 0, sDisplay);
        strcpy(sDisplay, "$T,  %02d m : %02d s   \r", tm,ts);
        Display_String(6, 0, 1, 0, sDisplay);
        if(PIND.4 == 1) {  
            PORTB = 0b00010000;
            ts++;
            delay_ms(100); }
        else if(PIND.5 == 1) { 
            tm++;   
            PORTB = 0b00100000;
            delay_ms(100); }
        else if(PIND.6 == 1) {  //  Continue with the existing timer. 
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
    strcpy(sDisplay, "$C\r");
    Display_String(6, 0, 1, 0, sDisplay);
    strcpy(sDisplay, "$B,0\r");
    Display_String(6, 0, 1, 0, sDisplay);
}
               
// Timer0 enables time to flow  
void timer0_ovf_isr(void)
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

// Timer2 : Timer settings using segments
void timer2_ovf_isr(void)
{
    TCNT2=0x06; 
    count2++;
    if(count2 == 1000){
        count2 = 0;     //  Given a timer value, it decreases by 1 per second.. 
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
            }       //  Find a way to pause by putting the current segment value in ts, tm.    
            ts = num10 * 10 + num1;
            tm = num1000 * 10 + num100;   
                        //  When '00 minutes 00 seconds', timer_alarm_enable becomes 1 and the alarm is output. 
            if(num1 == 0 && num10 == 0 && num100 == 0 && num1000 == 0){ 
                strcpy(sDisplay, "$C\r");
                Display_String(6, 0, 1, 0, sDisplay);
                timer_alarm_enable = 1;
                segment_enable = 0;//Set segment_enable to 0 so that there is no timer decrease. 
                ts = 0, tm = 0;    
            }
        }
    }
    PORTC = ~(1<<digit);
    if(digit == 0){ PORTA = data[num1000]; }
    else if( digit == 1) { PORTA = point_data[num100]; }   
     //  Expressing '.' in numbers in the third digit segment
     //   Minutes (m) and seconds (s) were distinguished. 
    else if( digit == 2) { PORTA = data[num10]; }           
    else { PORTA = data[num1]; }    
    delay_us(10);
    digit++;
    digit = digit%4;
    
}

// External interrupt 0 -> Current time setting interrupt 
void ext_int0_isr(void)
{
    PORTB = 0b00000001;
    strcpy(sDisplay, "$C\r");
    Display_String(6, 0, 1, 0, sDisplay);
    // Buttons 4 to 6, respectively, setting buttons for seconds (s) and hours (h). Button 7 saves and leaves the setting mode   
    // The LED of the corresponding bit is illuminated when each button is pressed.     
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
        strcpy(sDisplay, "$G, 1, 1\r");
        Display_String(6, 0, 1, 0, sDisplay);
        strcpy(sDisplay, "$T,Setting, SW 4-7\r");
        Display_String(6, 0, 1, 0, sDisplay);
        strcpy(sDisplay, "$G, 2, 1\r"); 
        Display_String(6, 0, 1, 0, sDisplay);
        strcpy(sDisplay, "$T, %02d : %02d : %02d   \r", h,m,s);
        Display_String(6, 0, 1, 0, sDisplay);
    }  
    strcpy(sDisplay, "$C\r");
    Display_String(6, 0, 1, 0, sDisplay);
}

// External interrupt 1 alarm clock setting interrupt
void ext_int1_isr(void)
{
    PORTB = 0b00000010;
    strcpy(sDisplay, "$C\r");
    Display_String(6, 0, 1, 0, sDisplay);
    alarm_setting_enable = 1;     //  To perform an alarm time setting function. 
       // If you set it up in the interrupt, the current time will stop, so in the while statement, 
       //  Check alarm_setting_enable = 1 to call the function. 
}

// External Interrupt 2: Alarm Intensity
void ext_int2_isr(void)
{
    PORTB = 0b00000100;
    strcpy(sDisplay, "$C\r");
    Display_String(6, 0, 1, 0, sDisplay);
    temp = read_adc(1);
    alarm_power = ((float)temp/1023)*(2); 
      //  Strength is specified in steps 1 to 3 according to variable resistance 
    power_enable = 1;
}

// External Interrupt 3: Set Segment Timer 
void ext_int3_isr(void)
{
    PORTB = 0b00001000;
    strcpy(sDisplay, "$C\r");  
    Display_String(6, 0, 1, 0, sDisplay);
    timer_setting_enable = 1;  
        // If you set it up in the interrupt, the current time will stop, so in the while statement,
       //  Check timer_setting_enable = 1 to call the function.
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
    // Timer settings   
    
    UCSR0A=(0<<RXC0) | (0<<TXC0) | (0<<UDRE0) | (0<<FE0) | (0<<DOR0) | (0<<UPE0) | (0<<U2X0) | (0<<MPCM0);
    UCSR0B=(0<<RXCIE0) | (0<<TXCIE0) | (0<<UDRIE0) | (0<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80);
    UCSR0C=(0<<UMSEL0) | (0<<UPM01) | (0<<UPM00) | (0<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00) | (0<<UCPOL0);
    UBRR0H=0x00;
    UBRR0L=0x67; 
    // UART0 settings 
    
    EICRA=(1<<ISC31) | (1<<ISC30) | (1<<ISC21) | (1<<ISC20) | (1<<ISC11) | (1<<ISC10) | (1<<ISC01) | (1<<ISC00);
    EICRB=(0<<ISC71) | (0<<ISC70) | (0<<ISC61) | (0<<ISC60) | (0<<ISC51) | (0<<ISC50) | (0<<ISC41) | (0<<ISC40);
    EIMSK=(0<<INT7) | (0<<INT6) | (0<<INT5) | (0<<INT4) | (1<<INT3) | (1<<INT2) | (1<<INT1) | (1<<INT0);
    EIFR=(0<<INTF7) | (0<<INTF6) | (0<<INTF5) | (0<<INTF4) | (1<<INTF3) | (1<<INTF2) | (1<<INTF1) | (1<<INTF0);       
    // External Interrupt Settings             
    
    ADMUX=ADC_VREF_TYPE;
    ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADFR) | (0<<ADIF) | (0<<ADIE) | (1<<ADPS2) | (0<<ADPS1) | (1<<ADPS0);
    SFIOR=(0<<ACME);
    // ADC settings
    
    #asm("sei")  
    
    DDRB = 0xff;
    PORTB = 0x00;
    // Port B : Press switch to output LED            
    
    DDRA = 0xff;
    DDRC = 0xff;    
    PORTC = 0b00001111; 
    // Segment Default Settings    
    
    DDRE = 0x0f;
    PORTE = 0x00;
    // DC Motor control    
    
    DDRF=(0<<DDF7) | (0<<DDF6) | (0<<DDF5) | (0<<DDF4) | (0<<DDF3) | (0<<DDF2) | (0<<DDF1) | (0<<DDF0);
    PORTF=(0<<PORTF7) | (0<<PORTF6) | (0<<PORTF5) | (0<<PORTF4) | (0<<PORTF3) | (0<<PORTF2) | (0<<PORTF1) | (0<<PORTF0);
    // Port F Settings for ADC 
    
    strcpy(sDisplay, "$I\r");
    Display_String(6, 0, 1, 0, sDisplay);
    strcpy(sDisplay, "$C\r"); 
    Display_String(6, 0, 1, 0, sDisplay);
    strcpy(sDisplay, "$L,0\r");
    Display_String(6, 0, 1, 0, sDisplay);
    strcpy(sDisplay, "$B,0\r");
    Display_String(6, 0, 1, 0, sDisplay);
    // LCD Initial Settings     
    
    temp = read_adc(1);
    alarm_power = ((float)temp/1023)*(5);  //  For initial alarm strength setting at first start

while (1)
    {                      
    strcpy(sDisplay,"$G, 1, 1\r");
    Display_String(6, 0, 1, 0, sDisplay);
    strcpy(sDisplay, "$T,   Time         \r");
    Display_String(6, 0, 1, 0, sDisplay);
    strcpy(sDisplay, "$G, 2, 1\r"); 
    Display_String(6, 0, 1, 0, sDisplay);
    strcpy(sdisplay, "$T, %02d : %02d : %02d   \r", h,m,s);
    Display_String(6, 0, 1, 0, sDisplay);
    // Current Time Output  
    
    PORTB = 0x00;  //  ?To prevent the LED and motor from operating under normal conditions.
    PORTE = 0x00;    
    
    if(alarm_setting_enable == 1){      //  Call alarm time setting function
        alarm_setting_function();
        alarm_setting_enable = 0;
    }       
    if(timer_setting_enable == 1) {    //  Call Timer Set Function
        timer_setting_function();
        timer_setting_enable = 0;
    }
    if(h == ah && m == am && alarm_enable == 1){ //   Alarm on the alarm clock
        alarm_output();
        alarm_enable = 0;     
    } 
    if (timer_alarm_enable == 1){    //  Timer alarm
        alarm_output();
        timer_alarm_enable = 0;    
    }          
    //  If you read the variable resistance value due to an interrupt,
    //  Call the print function to show the strength of the alarm. 
    if(power_enable == 1){ 
        power_print();
        power_enable = 0;
    }   
    if(PIND.5 == 1){     //  Press button 5 to turn the backlight off or on
        if (light == 0){
            strcpy(sDisplay, "$L,1\r");
            Display_String(6, 0, 1, 0, sDisplay);
            light = 1;
        }
        else if (light == 1) {
            strcpy(sDisplay, "$L,0\r");
            Display_String(6, 0, 1, 0, sDisplay);
            light = 0;
        }    
    }
    
    }   
}

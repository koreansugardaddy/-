//The DIY Life
//Michael Klements
//19 April 2020

#include <SPI.h>                //표시할 라이브러리 포함
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128        //OLED 디스플레이 너비(픽셀)
#define SCREEN_HEIGHT 32        //OLED 디스플레이 높이(픽셀)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);   //화면표시 객체를 작성합니다.

int pinButton = 14;             //버튼 & LED의 핀 정의
int pinLED = 15;

int startTime = 0;              //시작 및 종료 시간에 대한 변수 정의
int endTime = 0;

void setup() 
{
  pinMode (pinLED, OUTPUT);     //LED & 버튼 핀 할당
  pinMode (pinButton, INPUT);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) //디스플레이와 통신 시작, 128x32의 주소 0x3C
  { 
    for(;;);                    //진행 안 함, 디스플레이 통신이 실패할 경우 영구적으로 반복
  }
}

void loop() 
{
  display.clearDisplay();
  display.setTextSize(1);                     //Set the display text size
  display.setTextColor(SSD1306_WHITE);        //Draw white text
  display.setCursor(25,0);                    //Start at top-left corner, 25 pixels in
  display.println(F("Press To Play"));        //Display text on screen
  display.display();                          //Update display
  if (digitalRead(pinButton) == LOW)          //If button to start game is pressed
  {
      display.setCursor(40,10);               //Display "Ready" countdown with 3 sequential dots
      display.print(F("Ready"));
      display.display();
      delay(1000);
      display.print(F("."));
      display.display();
      delay(1000);
      display.print(F("."));
      display.display();
      delay(1000);
      display.print(F("."));
      display.display();
      int delayTime = random(2000,7000);      //Wait a random amount of time between 2s and 7s so that users can't guess the start time
      delay(delayTime);                       //Delay the random amount of time
      digitalWrite(pinLED, HIGH);             //Light up the LED
      startTime = millis();                   //Record the start time
      while (digitalRead(pinButton) == HIGH)  //Wait for the user to push the button
      {
        
      }
      endTime = millis();                     //Record the button push time
      digitalWrite(pinLED, LOW);              //Turn off the LED
      int totalTime = endTime - startTime;    //Calculate the total response time
      display.clearDisplay();                 //Display the results
      if (totalTime <= 20)                    //Check for cheating by holding the button down
      {
        display.setCursor(20,0);
        display.println(F("Don't Hold The"));
        display.setCursor(20,10);
        display.print(F("Button Down"));
      }
      else                                    //Display the results
      {
        display.setCursor(30,0);
        display.println(F("Your Time:"));
        display.setCursor(45,10);
        display.print(totalTime);
        display.print(F("ms"));
      }
      display.display();
      delay(5000);                             //Wait 5 seconds on results before restarting
  }
  delay(100);
}

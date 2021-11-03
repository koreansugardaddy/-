#include <SPI.h>                //표시할 라이브러리 포함
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128        //OLED 디스플레이 너비(픽셀)
#define SCREEN_HEIGHT 32        //OLED 디스플레이 높이(픽셀)

Adafruit_SSD1306 lcd(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);   //화면표시 객체를 작성합니다.

int btnOk = 6;             //버튼 & LED의 핀 정의
int pinLED = 13;

int startTime = 0;              //시작 및 종료 시간에 대한 변수 정의
int endTime = 0;

void setup() 
{
  pinMode (pinLED, OUTPUT);     //LED & 버튼 핀 할당
  pinMode (btnOk, INPUT);
  if(!lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C)) //디스플레이와 통신 시작, 128x32의 주소 0x3C
  { 
    for(;;);                    //진행 안 함, 디스플레이 통신이 실패할 경우 영구적으로 반복
  }
}

void loop() 
{
  lcd.clearDisplay();
  lcd.setTextSize(1);                     //표시 텍스트 크기 설정
  lcd.setTextColor(SSD1306_WHITE);        //흰색 텍스트 그리기
  lcd.setCursor(25,0);                    //왼쪽 상단 모서리에서 시작, 25픽셀
  lcd.println(F("Press To Play"));        //화면에 텍스트 표시
  lcd.display();                          //표시 업데이트
  if (digitalRead(btnOk) == LOW)          //게임 시작 버튼을 누르면
  {
      lcd.setCursor(40,10);               //3개의 순차적 점으로 "Ready" 카운트다운 표시
      lcd.print(F("Ready"));
      lcd.display();
      delay(1000);
      lcd.print(F("."));
      lcd.display();
      delay(1000);
      lcd.print(F("."));
      lcd.display();
      delay(1000);
      lcd.print(F("."));
      lcd.display();
      int delayTime = random(2000,7000);      //사용자가 시작 시간을 추측할 수 없도록 2초에서 7초 사이의 시간 동안 무작위로 기다립니다.
      delay(delayTime);                       //임의 시간 지연
      digitalWrite(pinLED, HIGH);             //LED를 켜십시오.
      startTime = millis();                   //시작 시간 기록
      while (digitalRead(btnOk) == HIGH)  //사용자가 버튼을 누를 때까지 기다립니다.
      {
        
      }
      endTime = millis();                     //버튼 누름 시간을 기록합니다.
      digitalWrite(pinLED, LOW);              //LED 끄기
      int totalTime = endTime - startTime;    //총 응답 시간 계산
      lcd.clearDisplay();                 //결과 표시
      if (totalTime <= 20)                    //버튼을 눌러 부정행위를 확인합니다.
      {
        lcd.setCursor(20,0);
        lcd.println(F("Don't Hold The"));
        lcd.setCursor(20,10);
        lcd.print(F("Button Down"));
      }
      else                                    //결과 표시
      {
        lcd.setCursor(30,0);
        lcd.println(F("Your Time:"));
        lcd.setCursor(45,10);
        lcd.print(totalTime);
        lcd.print(F("ms"));
      }
      lcd.display();
      delay(5000);                             //다시 시작하기 전에 결과를 5초 기다리십시오.
  }
  delay(100);
}

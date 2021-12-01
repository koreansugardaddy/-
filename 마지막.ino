#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <gfxfont.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SPI.h>



Adafruit_SSD1306 lcd(128, 64, &Wire, 4);

#define btnUp   4
#define btnDown 5
#define btnOk   6
#define btnBack 7
int startTime = 0;
int startTime1 = 0;              //시작 및 종료 시간에 대한 변수 정의
int startTime2 = 0;
int endTime = 0;
int endTime1 = 0;
int endTime2 = 0;
int red = 9;
int green = 10;
int blue = 11;

boolean statusBtnUp   = false;
boolean statusBtnOk   = false;
boolean statusBtnDown = false;
boolean statusBtnBack = false;

boolean statusAkhirBtnUp   = false;
boolean statusAkhirBtnOk   = false;
boolean statusAkhirBtnDown = false;
boolean statusAkhirBtnBack = false;

boolean UP   = false;
boolean OK   = false;
boolean DOWN = false;
boolean BACK = false;

int halaman  = 1;
int menuItem = 1;

void setup() {
  Serial.begin(115200);
  lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  lcd.clearDisplay();
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(btnUp,   INPUT_PULLUP);
  pinMode(btnOk,   INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  pinMode(btnBack, INPUT_PULLUP);
  if(!lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C)) //디스플레이와 통신 시작, 128x32의 주소 0x3C
  { 
    for(;;);                    //진행 안 함, 디스플레이 통신이 실패할 경우 영구적으로 반복
  }
}

void loop() {
  tampil();

  statusBtnUp   = digitalRead(btnUp);
  statusBtnOk   = digitalRead(btnOk);
  statusBtnDown = digitalRead(btnDown);
  statusBtnBack = digitalRead(btnBack);

  saatUpDitekan();
  saatOkDitekan();
  saatDownDitekan();
  saatBackDitekan();

  //  untuk button up
  if (UP && halaman == 1) {
    UP = false;
    menuItem --;
    if (menuItem < 1)menuItem = 4;
  }
  //untuk button down
  if (DOWN && halaman == 1) {
    DOWN = false;
    menuItem ++;
    if (menuItem > 4)menuItem = 1;
  }

  //  untuk button ok
  if (OK) {
    OK = false;
    if (halaman == 1 && menuItem == 1) {
      halaman = 2;
    } else if (halaman == 1 && menuItem == 2) {
      halaman = 3;
    } else if (halaman == 1 && menuItem == 3) {
      halaman = 4;
    } else if (halaman == 1 && menuItem == 4) {
      halaman = 5;
    }
  }

  //  untuk button back
  if (BACK) {
    BACK = false;
    if (halaman == 2 || halaman == 3 || halaman == 4 || halaman == 5) {
      halaman = 1;
    }
  }

  Serial.println(menuItem);
  delay(100);
}

//--------------------------------------------------------------------------------
void saatUpDitekan() {
  if (statusBtnUp != statusAkhirBtnUp) {
    if (statusBtnUp == 0) {
      UP = true;
    }
    delay(50);
  }
  statusAkhirBtnUp = statusBtnUp;
}

void saatOkDitekan() {
  if (statusBtnOk != statusAkhirBtnOk) {
    if (statusBtnOk == 0) {
      OK = true;
    }
    delay(50);
  }
  statusAkhirBtnOk = statusBtnOk;
}

void saatDownDitekan() {
  if (statusBtnDown != statusAkhirBtnDown) {
    if (statusBtnDown == 0) {
      DOWN = true;
    }
    delay(50);
  }
  statusAkhirBtnDown = statusBtnDown;
}

void saatBackDitekan() {
  if (statusBtnBack != statusAkhirBtnBack) {
    if (statusBtnBack == 0) {
      BACK = true;
    }
    delay(50);
  }
  statusAkhirBtnBack = statusBtnBack;
}
//----------------------------------------------------------------------------

//semua yang tampil di lcd ada di fungsi ini

void tampil() {
  if (halaman == 1) {
    lcd.clearDisplay();
    lcd.setTextSize(1);
    lcd.setTextColor(WHITE);
    lcd.setCursor(52, 0);
    lcd.print("menu");

    if (menuItem == 1) {
      lcd.setCursor(30, 17);
      lcd.setTextColor(WHITE);
      lcd.print("> 1 Player");
    } else {
      lcd.setCursor(30, 17);
      lcd.setTextColor(WHITE);
      lcd.print("  1 Player");
    }

    if (menuItem == 2) {
      lcd.setCursor(30, 27);
      lcd.setTextColor(WHITE);
      lcd.print("> 2 Player");
    } else {
      lcd.setCursor(30, 27);
      lcd.setTextColor(WHITE);
      lcd.print("  2 Player");
    }

    if (menuItem == 3) {
      lcd.setCursor(5, 37);
      lcd.setTextColor(WHITE);
      lcd.print("> menuItem 3");
    } else {
      lcd.setCursor(5, 37);
      lcd.setTextColor(WHITE);
      lcd.print("  menuItem 3");
    }

    if (menuItem == 4) {
      lcd.setCursor(5, 47);
      lcd.setTextColor(WHITE);
      lcd.print("> menuItem 4");
    } else {
      lcd.setCursor(5, 47);
      lcd.setTextColor(WHITE);
      lcd.print("  menuItem 4");
    }

  } 
  else if (halaman == 2) {
    lcd.clearDisplay();
    lcd.setTextSize(2);
    lcd.setTextColor(WHITE);
    {
    lcd.clearDisplay();
    lcd.setTextSize(1);                     //표시 텍스트 크기 설정
    lcd.setTextColor(SSD1306_WHITE);        //흰색 텍스트 그리기
    lcd.setCursor(25,20);                    //왼쪽 상단 모서리에서 시작, 25픽셀
    lcd.println(F("Press To Play"));        //화면에 텍스트 표시
    lcd.display();                          //표시 업데이트
    if (digitalRead(btnOk) == LOW)          //게임 시작 버튼을 누르면
    {
        lcd.setCursor(50,30);               //3개의 순차적 점으로 "Ready" 카운트다운 표시
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
        digitalWrite(blue, HIGH);             //LED를 켜십시오.
        startTime = millis();                   //시작 시간 기록
        while (digitalRead(btnOk) == HIGH)  //사용자가 버튼을 누를 때까지 기다립니다.
        {
          
        }
        endTime = millis();                     //버튼 누름 시간을 기록합니다.
        digitalWrite(blue, LOW);              //LED 끄기
        int totalTime = endTime - startTime;    //총 응답 시간 계산
        lcd.clearDisplay();                 //결과 표시
        if (totalTime <= 20)                    //버튼을 눌러 부정행위를 확인합니다.
        {
          lcd.setCursor(25,20);
          lcd.println(F("Don't Hold The"));
          lcd.setCursor(20,30);
          lcd.print(F("Button Down"));
        }
        else                                    //결과 표시
        {
          lcd.setCursor(30,20);
          lcd.println(F("Your Time:"));
          lcd.setCursor(45,30);
          lcd.print(totalTime);
          lcd.print(F("ms"));
          {
            if(totalTime>=0 && totalTime<=50)
                {
                  lcd.setCursor(55,40);
                  lcd.print("5%");
                 }
            else if(totalTime>=26 && totalTime<=50)
            {
              lcd.setCursor(55,40);
              lcd.print("10%");
            }
            else if(totalTime>=51 && totalTime<=75)
            {
              lcd.setCursor(55,40);
              lcd.print("15%");
            }
            else if(totalTime>=76 && totalTime<=100)
            {
              lcd.setCursor(55,40);
              lcd.print("20%");
            }
            else if(totalTime>=101 && totalTime<=125)
            {
              lcd.setCursor(55,40);
              lcd.print("25%");
            }
            else if(totalTime>=126 && totalTime<=150)
            {
              lcd.setCursor(55,40);
              lcd.print("30%");
            }else if(totalTime>=151 && totalTime<=175)
            {
              lcd.setCursor(55,40);
              lcd.print("35%");
            }else if(totalTime>=176 && totalTime<=200)
            {
              lcd.setCursor(55,40);
              lcd.print("40%");
            }
            else if(totalTime>=201 && totalTime<=225)
            {
              lcd.setCursor(55,40);
              lcd.print("45%");
            }
            else if(totalTime>=226 && totalTime<=250)
            {
              lcd.setCursor(55,40);
              lcd.print("50%");
            }
            else if(totalTime>=251 && totalTime<=275)
            {
              lcd.setCursor(55,40);
              lcd.print("55%");
            }
            else if(totalTime>=276 && totalTime<=300)
            {
              lcd.setCursor(55,40);
              lcd.print("60%");
            }
            else if(totalTime>=301 && totalTime<=325)
            {
              lcd.setCursor(55,40);
              lcd.print("65%");
            }
            else if(totalTime>=326 && totalTime<=350)
            {
              lcd.setCursor(55,40);
              lcd.print("70%");
            }
            else if(totalTime>=351 && totalTime<=375)
            {
              lcd.setCursor(55,40);
              lcd.print("75%");
            }else if(totalTime>=376 && totalTime<=400)
            {
              lcd.setCursor(55,40);
              lcd.print("80%");
            }else if(totalTime>=401 && totalTime<=425)
            {
              lcd.setCursor(55,40);
              lcd.print("85%");
            }else if(totalTime>=426 && totalTime<=450)
            {
              lcd.setCursor(55,40);
              lcd.print("90%");
            }else if(totalTime>=451 && totalTime<=475)
            {
              lcd.setCursor(55,40);
              lcd.print("95%");
            }else
            {
              lcd.setCursor(55,40);
              lcd.print("100%");
            }
          }
        }
        lcd.display();
        delay(5000);                             //다시 시작하기 전에 결과를 5초 기다리십시오.
    }
    delay(100);
  }
  
    }
  else if (halaman == 3) 
  {
    lcd.clearDisplay();
    lcd.setTextSize(2);
    lcd.setTextColor(WHITE);
    {
      lcd.clearDisplay();
      lcd.setTextSize(1);                     //표시 텍스트 크기 설정
      lcd.setTextColor(SSD1306_WHITE);        //흰색 텍스트 그리기
      lcd.setCursor(25,20);                    //왼쪽 상단 모서리에서 시작, 25픽셀
      lcd.println(F("Press To Play"));        //화면에 텍스트 표시
      lcd.display();                           //표시 업데이트
      if (digitalRead(btnOk) == LOW)          //게임 시작 버튼을 누르면
      {
      lcd.setCursor(50,30);               //3개의 순차적 점으로 "Ready" 카운트다운 표시
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
      digitalWrite(blue, HIGH);             //LED를 켜십시오.
      startTime1 = millis();
      while(digitalRead(btnUp) == HIGH)
      {

        endTime1 = millis(); 
      }
      startTime2 = millis();
      while(digitalRead(btnDown) == HIGH)
      {

        endTime2 = millis(); 
      }
      digitalWrite(blue, LOW);              //LED 끄기
      int totalTime1 = endTime1 - startTime1;    //총 응답 시간 계산
      int totalTime2 = endTime2 - startTime2;
        if (totalTime1 <= 20)                    //버튼을 눌러 부정행위를 확인합니다.
        {
          lcd.clearDisplay();
          lcd.setCursor(25,20);
          lcd.println(F("Don't Hold The"));
          lcd.setCursor(20,30);
          lcd.print(F("Button Down"));
        }
       if (totalTime2 <= 20)                    //버튼을 눌러 부정행위를 확인합니다.
        {
          lcd.clearDisplay();
          lcd.setCursor(25,20);
          lcd.println(F("Don't Hold The"));
          lcd.setCursor(20,30);
          lcd.print(F("Button Down"));
        }
        else if(totalTime1 <= totalTime2)                                  //결과 표시
        {
          lcd.clearDisplay();
          lcd.setCursor(30,20);
          lcd.println(F("Your Time:"));
          lcd.setCursor(45,30);
          lcd.print(totalTime2);
          lcd.print(F("ms"));
          lcd.setCursor(45,40);
          lcd.print("player 1 win");
        }
        else if(totalTime2 <= totalTime1)
        {
          lcd.clearDisplay();
          lcd.setCursor(30,20);
          lcd.println(F("Your Time:"));
          lcd.setCursor(45,30);
          lcd.print(totalTime2);
          lcd.print(F("ms"));
          lcd.setCursor(45,40);
          lcd.print("player2 win");
        }
      }
      lcd.display();
      delay(5000);                             //다시 시작하기 전에 결과를 5초 기다리십시오.
  }
  delay(100);
}
  else if (halaman == 4) 
  {
    lcd.clearDisplay();
    lcd.setTextSize(2);
    lcd.setTextColor(WHITE);
    lcd.setCursor(25, 25);
    lcd.print("ini hal 4");
  } 

  lcd.display();
}

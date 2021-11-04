
// SCK = A5 SDA = A4

#include <SPI.h>  
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>


Adafruit_SSD1306 lcd(128, 64, &Wire, 4);

#define btnUp   7
#define btnOk   6
#define btnDown 5
#define btnBack 4
int startTime = 0;              //시작 및 종료 시간에 대한 변수 정의
int endTime = 0;
int pinLED = 13;

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
  
  pinMode (pinLED, OUTPUT);
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
      lcd.setCursor(40, 17);
      lcd.setTextColor(WHITE);
      lcd.print("> 1P game");
    } else {
      lcd.setCursor(40, 17);
      lcd.setTextColor(WHITE);
      lcd.print("  1P game");
    }

    if (menuItem == 2) {
      lcd.setCursor(40, 27);
      lcd.setTextColor(WHITE);
      lcd.print("> 2P game");
    } else {
      lcd.setCursor(40, 27);
      lcd.setTextColor(WHITE);
      lcd.print("  2P game");
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

  } else if (halaman == 2) {
    lcd.clearDisplay();
    lcd.setTextSize(2);
    lcd.setTextColor(WHITE);
    lcd.setCursor(25, 25);
    lcd.print("ImamID");
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
  } else if (halaman == 3) {
    lcd.clearDisplay();
    lcd.setTextSize(2);
    lcd.setTextColor(WHITE);
    lcd.setCursor(10, 15);
    lcd.print("SUBSCRIBE");
    lcd.setCursor(30, 35);
    lcd.print("ImamID");
  } else if (halaman == 4) {
    lcd.clearDisplay();
    lcd.setTextSize(2);
    lcd.setTextColor(WHITE);
    lcd.setCursor(25, 25);
    lcd.print("ini hal 4");
  } else if (halaman == 5) {
    lcd.clearDisplay();
    lcd.setTextSize(2);
    lcd.setTextColor(WHITE);
    lcd.setCursor(25, 25);
    lcd.print("ini hal 5");
  }

  lcd.display();
}

#include <SPI.h>  
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>


Adafruit_SSD1306 lcd(128, 64, &Wire, 4);

#define btnUp   4
#define btnOk   6
#define btnDown 5
#define btnBack 7
int startTime = 0;              //시작 및 종료 시간에 대한 변수 정의
int endTime = 0;
int red = 9;
int green = 10;
int blue = 11;
int buttonStateP1 =0 ;//Player 1 button tracking 
int buttonStateP2 =0 ;//Player 2 button tracking 
int lastButtonStateP1 =0 ;// Player 1 last button state (used to detect a button press)
int lastButtonStateP2 =0 ;// Player 2 last button state (used to detect a button press)
int scoreP1 =0 ;// Player 1 score tracking 
int scoreP2 =0 ;// Player 2 score tracking 

boolean statusBtnUp   = false;
boolean statusBtnOk   = false;
boolean statusBtnDown = false;
boolean statusBtnBack = false;

boolean statusAkhirBtnUp   = false;
boolean statusAkhirBtnOk   = false;
boolean statusAkhirBtnDown = false;
boolean statusAkhirBtnBack = false;

boolean bothDone =false ;// 두 플레이어가 모두 버튼을 눌렀는지 확인하는 데 사용됩니다.
boolean gameOn =false ;// 게임 진행 여부를 추적합니다.
boolean startButtonState =LOW ;// 시작 단추 초기화
boolean p1Done =false ;// 플레이어 1의 단추 누르기 추적
boolean p2Done =false ;// 플레이어 2의 단추 누르기 추적 
boolean welcomeMsg =false ;// 시작 메시지가 이미 표시된 경우 추적

boolean UP   = false;
boolean OK   = false;
boolean DOWN = false;
boolean BACK = false;

long randomTime ;// 게임 시작 후 표시등이 다시 켜질 때까지 임의의 시간을 유지합니다. 
long startTime1 ;// 그 게임은 언제 시작되었나요?t 
long endTimeP1 ;// 플레이어 1은 언제 그들의 버튼을 눌렀습니까? 
long endTimeP2 ;// 플레이어 2는 언제 버튼을 눌렀습니까?

int finalTimeP1 ;// 게임 시작 후 플레이어 1이 버튼을 누를 때까지 경과한 시간 
int finalTimeP2 ;// 게임 시작 후 플레이어 2가 버튼을 누를 때까지 경과한 시간 
int winningTime ;// 승자와 패자 사이의 시간 
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

  } else if (halaman == 2) {
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

  } else if (halaman == 3) {
    lcd.clearDisplay();
    lcd.setTextSize(2);
    lcd.setTextColor(WHITE);
    {
      // 현재 플레이어의 점수인 시작 메시지를 인쇄하고 시작 메시지가 표시되도록 설정합니다. 
      if (welcomeMsg ==false )
      {
        Serial .println (" ");
        Serial .println ("Press the start button to begin");
        Serial .println ("Scoreboard");
        Serial .print ("1 Player - ");
        Serial .println (scoreP1);
        Serial .print ("2 Player - ");
        Serial .println (scoreP2);
        welcomeMsg =true ;
      }
       startButtonState =digitalRead (btnOk );// 시작 버튼을 누를 때까지 듣습니다.
       
  
  // 시작 버튼을 눌렀지만 이미 실행 중인 게임이 없다면 게임을 시작하십시오. 
       if (startButtonState ==HIGH &&gameOn ==false )         // LED가 다시 켜지는 동안 게임 시작 간 지연 시간을 임의로 생성합니다.
       {
        Serial .println (" ");
        Serial .println ("Get ready!");
        int randomTime = random(2000,7000);      //사용자가 시작 시간을 추측할 수 없도록 2초에서 7초 사이의 시간 동안 무작위로 기다립니다.
        delay(randomTime); 
        digitalWrite (red ,HIGH );// 게임 LED를 켜고 끄면 게임이 시작됨을 알 수 있습니다. 
        delay (1000 );
        digitalWrite (red ,LOW );
        delay (randomTime);
   
        gameOn =true ;// 현재 실행 중인 게임을 선언합니다. 
        startTime1 =millis ();
        digitalWrite (red ,HIGH);// 가능한 한 빨리 버튼을 눌러야 함을 나타내는 게임 LED 켜기 
        while (p1Done ==false ||p2Done ==false)
        {
          buttonStateP1 =digitalRead (btnUp);
          buttonStateP2 =digitalRead (btnDown);
          // 플레이어 1 버튼을 누를 때까지 듣고 플레이어 1을 설정합니다.
          if (buttonStateP1 ==HIGH &&p1Done ==false)
          {
            endTimeP1 =millis ();
            p1Done =true ;     
          }
           // 플레이어 2 버튼을 누를 때까지 듣고 플레이어 2를 설정하십시오.
        if (buttonStateP2 ==HIGH &&p2Done ==false)
          {
            endTimeP2 =millis ();
            p2Done =true ;  
          }
        }

        digitalWrite (red ,LOW );// 게임 LED 끄기
        finalTimeP1 =(endTimeP1 -startTime1 );  //플레이어에서 버튼을 누르는 데 걸린 시간 계산 
        finalTimeP2 =(endTimeP2 -startTime1 );  //플레이어에서 버튼을 누르는 데 걸린 시간 계산
                                                // 플레이어 1의 마지막 시간(초) 표시 
                                               // Display Player 2's final time in seconds 
        if (endTimeP1 <endTimeP2 )              // Run if Player 1 won the round
        {
          digitalWrite (blue ,HIGH );
          scoreP1 =scoreP1 +1 ;
          Serial .print("1 Player win!!");
          Serial .println(" ");
          Serial .print(finalTimeP1);
          Serial .print(" ms");
          digitalWrite (blue ,HIGH );
          digitalWrite (green ,LOW );
        }
        else 
        {
          scoreP2 =scoreP2 +1 ;
          Serial .print("2 Player win!!");
          Serial .println (" ");
          Serial .print(finalTimeP2);
          Serial .print(" ms");
          digitalWrite (green ,HIGH );
          digitalWrite (blue ,LOW );
        }
        delay (5000 );
        digitalWrite (blue ,LOW );// Turn of Player 1's LED 
        digitalWrite (green ,LOW );// Turn of Player 2's LED 

        // Reset all variables to restart the game 
        buttonStateP1 =0 ;
        buttonStateP2 =0 ; 
        lastButtonStateP1 =0 ; 
        lastButtonStateP2 =0 ; 
        bothDone =false ;
        gameOn =false ;
        startButtonState =LOW ;
        p1Done =false ;
        p2Done =false ;
        randomTime =0 ;
        startTime1 =0 ;
        endTimeP1 =0 ;
        endTimeP2 =0 ;
        finalTimeP1 =0 ;
        finalTimeP2 =0 ;
        winningTime =0 ;
        welcomeMsg =false ;
      }
  lcd.display();
    }
  }
}

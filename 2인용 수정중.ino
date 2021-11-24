#include <SPI.h>  
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

Adafruit_SSD1306 lcd(128, 64, &Wire, 4);

const int btnOk =6 ;//Player 1 button 
const int btnBack =7 ;//Player 2 button 

int switchStart =1 ;//Start button 
int green =10 ;//Game start LED 
int red =9 ;//Player 1 win indicator 
int blue =11 ;//Player 2 win indicator 
int buttonStateP1 =0 ;//Player 1 button tracking 
int buttonStateP2 =0 ;//Player 2 button tracking 
int lastButtonStateP1 =0 ;// Player 1 last button state (used to detect a button press)
int lastButtonStateP2 =0 ;// Player 2 last button state (used to detect a button press)
int scoreP1 =0 ;// Player 1 score tracking 
int scoreP2 =0 ;// Player 2 score tracking 

boolean bothDone =false ;// 두 플레이어가 모두 버튼을 눌렀는지 확인하는 데 사용됩니다.
boolean gameOn =false ;// 게임 진행 여부를 추적합니다.
boolean startButtonState =LOW ;// 시작 단추 초기화
boolean p1Done =false ;// 플레이어 1의 단추 누르기 추적
boolean p2Done =false ;// 플레이어 2의 단추 누르기 추적 
boolean welcomeMsg =false ;// 시작 메시지가 이미 표시된 경우 추적

long randomTime ;// 게임 시작 후 표시등이 다시 켜질 때까지 임의의 시간을 유지합니다. 
long startTime ;// 그 게임은 언제 시작되었나요?t 
long endTimeP1 ;// 플레이어 1은 언제 그들의 버튼을 눌렀습니까? 
long endTimeP2 ;// 플레이어 2는 언제 버튼을 눌렀습니까? 

int finalTimeP1 ;// 게임 시작 후 플레이어 1이 버튼을 누를 때까지 경과한 시간 
int finalTimeP2 ;// 게임 시작 후 플레이어 2가 버튼을 누를 때까지 경과한 시간 
int winningTime ;// 승자와 패자 사이의 시간 

void setup ()
{
  pinMode (btnOk ,INPUT );
  pinMode (btnBack ,INPUT );
  pinMode (green ,OUTPUT );// 게임 시작 LED
  pinMode (red ,OUTPUT );// Player 1 win LED 
  pinMode (blue ,OUTPUT );// Player 1 win LED 
  Serial.begin(115200);
  lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  lcd.clearDisplay();
}

void loop (){
  // 현재 플레이어의 점수인 시작 메시지를 인쇄하고 시작 메시지가 표시되도록 설정합니다. 
  if (welcomeMsg ==false ){
    lcd.println (" ");
    lcd.println ("Press the start button to begin");
    lcd.println ("Scoreboard");
    lcd.println ("1 Player - ");
    lcd.println (scoreP1);
    lcd.println ("2 Player - ");
    lcd.println (scoreP2);
    welcomeMsg =true ;
  } 
  startButtonState =digitalRead (switchStart );// 시작 버튼을 누를 때까지 듣습니다.
  
  // 시작 버튼을 눌렀지만 이미 실행 중인 게임이 없다면 게임을 시작하십시오. 
  if (startButtonState ==HIGH &&gameOn ==false ){
    Random ();
  }
}

// LED가 다시 켜지는 동안 게임 시작 간 지연 시간을 임의로 생성합니다.
void Random (){
  lcd.println (" ");
  lcd.println ("Get ready!");
  randomTime =random (4 ,10 );
  randomTime =randomTime *1000 ;
  digitalWrite (ledPin ,HIGH );// 게임 LED를 켜고 끄면 게임이 시작됨을 알 수 있습니다. 
  delay (1000 );
  digitalWrite (ledPin ,LOW );
  delay (randomTime );
  startGame ();
}

// 플레이어 1 및 플레이어 2 단추가 눌릴 때까지 듣습니다. 
void startGame (){
  gameOn =true ;// 현재 실행 중인 게임을 선언합니다. 
  startTime =millis ();
  digitalWrite (ledPin ,HIGH );// 가능한 한 빨리 버튼을 눌러야 함을 나타내는 게임 LED 켜기 
  while (p1Done ==false ||p2Done ==false ){
    buttonStateP1 =digitalRead (switchPin1 );
    buttonStateP2 =digitalRead (switchPin2 );
    // 플레이어 1 버튼을 누를 때까지 듣고 플레이어 1을 설정합니다.
    if (buttonStateP1 ==HIGH &&p1Done ==false ){
      endTimeP1 =millis ();
      p1Done =true ;     
    }
    // 플레이어 2 버튼을 누를 때까지 듣고 플레이어 2를 설정하십시오.
    if (buttonStateP2 ==HIGH &&p2Done ==false ){
      endTimeP2 =millis ();
      p2Done =true ;  
    }
  }
  endGame ();
}

void endGame (){
  digitalWrite (green ,LOW );// 게임 LED 끄기
  finalTimeP1 =(endTimeP1 -startTime );//플레이어에서 버튼을 누르는 데 걸린 시간 계산 
  finalTimeP2 =(endTimeP2 -startTime );//플레이어에서 버튼을 누르는 데 걸린 시간 계산

// 플레이어 1의 마지막 시간(초) 표시 

// Display Player 2's final time in seconds 

  if (endTimeP1 <endTimeP2 ){// Run if Player 1 won the round 
    digitalWrite (red ,HIGH );
    scoreP1 =scoreP1 +1 ;
    lcd.println("Player 1 win!!");
    lcd.println(" ");
    lcd.println(finalTimeP1);
    lcd.println(" ms");
    digitalWrite (red ,HIGH );
    digitalWrite (blue ,LOW );
  }
  else {
    scoreP2 =scoreP2 +1 ;
    lcd.println("Player 2 win!!");
    lcd.println (" ");
    lcd.println(finalTimeP2);
    lcd.println(" ms");
    digitalWrite (blue ,HIGH );
    digitalWrite (red ,LOW );
  }
  delay (5000 );
  digitalWrite (red ,LOW );// Turn of Player 1's LED 
  digitalWrite (blue ,LOW );// Turn of Player 2's LED 
  
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
  startTime =0 ;
  endTimeP1 =0 ;
  endTimeP2 =0 ;
  finalTimeP1 =0 ;
  finalTimeP2 =0 ;
  winningTime =0 ;
  welcomeMsg =false ;
}

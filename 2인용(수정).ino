#include <SPI.h>  
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>


Adafruit_SSD1306 lcd(128, 64, &Wire, 4);

const int switchPin1 =6 ;//Player 1 button 
const int switchPin2 =7 ;//Player 2 button 

int switchStart =5 ;//Start button 
int ledPin =9 ;//Game start LED 
int ledPinP1 =10 ;//Player 1 win indicator 
int ledPinP2 =11 ;//Player 2 win indicator 
int buttonStateP1 =0 ;//Player 1 button tracking 
int buttonStateP2 =0 ;//Player 2 button tracking 
int lastButtonStateP1 =0 ;// Player 1 last button state (used to detect a button press)
int lastButtonStateP2 =0 ;// Player 2 last button state (used to detect a button press)
int scoreP1 =0 ;// Player 1 score tracking 
int scoreP2 =0 ;// Player 2 score tracking 

boolean bothDone =false ;// Used to see if both players have pressed their buttons 
boolean gameOn =false ;// Keep track if the game is going on or not 
boolean startButtonState =LOW ;// Start button initalization 
boolean p1Done =false ;// Keep track of Player 1's button press 
boolean p2Done =false ;// Keep track of Player 1's button press 
boolean welcomeMsg =false ;// Keep track if the welcome message has already been displayed 

long randomTime ;// Hold the random time between the start of the game and the indicator light coming back on 
long startTime ;// When did the game start 
long endTimeP1 ;// When did Player 1 press their button 
long endTimeP2 ;// When did Player 2 press their button 

float finalTimeP1 ;// Time elapsed between start of the game and Player 1 pressing their button 
float finalTimeP2 ;// Time elapsed between start of the game and Player 2 pressing their button 
float winningTime ;// Time between the winning and losing player's time 

void setup ()
{
  Serial.begin(115200);
  lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  lcd.clearDisplay();
  pinMode (switchPin1 ,INPUT );
  pinMode (switchPin2 ,INPUT );
  pinMode (ledPin ,OUTPUT );// Game start LED 
  pinMode (ledPinP1 ,OUTPUT );// Player 1 win LED 
  pinMode (ledPinP2 ,OUTPUT );// Player 1 win LED 
  Serial .begin (9600 );
  if(!lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C)) //디스플레이와 통신 시작, 128x32의 주소 0x3C
  { 
    for(;;);                    //진행 안 함, 디스플레이 통신이 실패할 경우 영구적으로 반복
  }
}

void loop ()
{
  // Print a welcome message, the current player's score, and set that the welcome message has been displayed 
  if (welcomeMsg ==false ){
    lcd.clearDisplay (" ");
    lcd.clearDisplay ("Press the start button to begin");
    lcd.clearDisplay ("Scoreboard:");
    lcd.setCursor(52, 0);
    lcd.print ("Player 1 - ");
    lcd..println (scoreP1 );
    lcd.print ("Player 2 - ");
    lcd.println (scoreP2 );
    welcomeMsg =true ;
  } 
  startButtonState =digitalRead (switchStart );// Listen for the start button to be pressed 
  
  // if the start button has been pressed and there is no game already running, begin the game 
  if (startButtonState ==HIGH &&gameOn ==false ){
    Random ();
  }
}

// Generate a random ammount of time to delay between the begining of the game intil the LED comes back on 
void Random (){
  Serial .println (" ");
  Serial .println ("Get ready!");
  randomTime =random (4 ,10 );
  randomTime =randomTime *1000 ;
  digitalWrite (ledPin ,HIGH );// Turn the game LED on and off to indicate a game is about to start 
  delay (1000 );
  digitalWrite (ledPin ,LOW );
  delay (randomTime );
  startGame ();
}

// Listen for the Player 1 and Player 2 buttons to be pressed 
void startGame (){
  gameOn =true ;// Declare a game currently running 
  startTime =millis ();
  digitalWrite (ledPin ,HIGH );// Turn on game LED indicating players should press their buttons as quickly as possible 
  while (p1Done ==false ||p2Done ==false ){
    buttonStateP1 =digitalRead (switchPin1 );
    buttonStateP2 =digitalRead (switchPin2 );
    // Listen for Player 1 button to be pressed and set Player 1 as done.
    if (buttonStateP1 ==HIGH &&p1Done ==false ){
      endTimeP1 =millis ();
      p1Done =true ;     
    }
    // Listen for Player 2 button to be pressed and set Player 2 as done.
    if (buttonStateP2 ==HIGH &&p2Done ==false ){
      endTimeP2 =millis ();
      p2Done =true ;  
    }
  }
  endGame ();
}

void endGame (){
  digitalWrite (ledPin ,LOW );// Turn off the game LED 
  finalTimeP1 =(endTimeP1 -startTime );//Calculate how long it took Player to push their button 
  finalTimeP2 =(endTimeP2 -startTime );//Calculate how long it took Player to push their button 
  Serial .print ("P1 time:"); 
  Serial .println (finalTimeP1 /1000 );// Display Player 1's final time in seconds 
  Serial .print ("P2 time:");
  Serial .println (finalTimeP2 /1000 );// Display Player 2's final time in seconds 

  if (endTimeP1 <endTimeP2 ){// Run if Player 1 won the round 
    digitalWrite (ledPinP1 ,HIGH );
    winningTime =(endTimeP2 -startTime )-(endTimeP1 -startTime );
    scoreP1 =scoreP1 +1 ;
    Serial .print ("P1 won by:");
    Serial .println (winningTime /1000 );
    digitalWrite (ledPinP1 ,HIGH );
    digitalWrite (ledPinP2 ,LOW );
  }
  else {
    Serial .print ("P2 won by:");// Run if Player 2 won te round 
    winningTime =(endTimeP1 -startTime )-(endTimeP2 -startTime );
    scoreP2 =scoreP2 +1 ;
    Serial .println (winningTime /1000 );
    digitalWrite (ledPinP2 ,HIGH );
    digitalWrite (ledPinP1 ,LOW );
  }
  delay (5000 );
  digitalWrite (ledPinP1 ,LOW );// Turn of Player 1's LED 
  digitalWrite (ledPinP2 ,LOW );// Turn of Player 2's LED 
  
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

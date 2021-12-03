// set pin numbers:
const int buttonPin1 = 6;     // the number of the pushbutton1 pin
const int buttonPin2 = 7;     // the number of the pushbutton2 pin
const int buttonPin3 = 2;     // the number of the pushbutton3 pin
const int ledPin1 = 8;        // the number of the LED1 anode(+) pin
const int ledPin2 = 9;       // the number of the LED2 anode(+) pin

// Variables will change:
int ledState1 = LOW;         // the current state of the LED1
int ledState2 = LOW;         // the current state of the LED2
int buttonState1;             // the current reading from the input pin1
int buttonState2;             // the current reading from the input pin2
int buttonState3;             // the current reading from the input pin3
int lastButtonState1 = LOW;    // the previous reading from the input pin1
int lastButtonState2 = LOW;    // the previous reading from the input pin2
int lastButtonState3 = LOW;    // the previous reading from the input pin3


// the following variables are long's because the time, measured in microseconds,
// will quickly become a bigger number than can be stored in an int.

unsigned long lastDebounceTime = 0;   // the last time the output pin was toggled
long debounceDelay = 20000;           // the debounce time in micro second

unsigned long randNumber;                 // Generated random number
unsigned long minRandomNumber = 2000;     // minimum number used to specify the range of random number
unsigned long maxRandomNumber = 5000;     // maximum number used to specify the range of random number

unsigned long time1,time2;
int button3Pressed = LOW;
int printcount = 0;

int takeReading = LOW;

//following variable help in reading buttons pins
int reading1;
int reading2;
int reading3;

//following variable help in reading button corresponding to perticular led i.e LED1 --> BUTTON1 and LED2 --> BUTTON2
int oddNumber ;
int evenNumber ;

void setup()
{
      pinMode(buttonPin1, INPUT);
      pinMode(buttonPin2, INPUT);
      pinMode(buttonPin3, INPUT);
      pinMode(ledPin1, OUTPUT);
      pinMode(ledPin2, OUTPUT);
      
      //enabling serial communication
      Serial.begin(115200);
      
      // set initial LED state
      digitalWrite(ledPin1, ledState1);
      digitalWrite(ledPin2, ledState2);
    
      // if analog input pin 0 is unconnected, random analog
      // noise will cause the call to randomSeed() to generate
      // different seed numbers each time the sketch runs.
      // randomSeed() will then shuffle the random function.
      randomSeed(analogRead(0));
}
    
void loop() 
{
      if( printcount == 0)
      {
          Serial.println("press button 3 when you are ready");
          printcount = 1;
          takeReading = LOW;    // not to read button number 1 and 2
          oddNumber = LOW;
          evenNumber = LOW;
      }
      
      if( button3Pressed == HIGH)
      {
          digitalWrite(ledPin1, LOW);
          digitalWrite(ledPin2, LOW);
          randNumber = random(minRandomNumber,maxRandomNumber);     //in our code we have kept them as 2000 to 5000
          delay(randNumber);
          if ( randNumber & 1 == 1)
          {
              digitalWrite(ledPin1, HIGH);
              digitalWrite(ledPin2, LOW);
              time1 = micros();
              //Serial.println("ODD");
              oddNumber = HIGH;
          }
          else
          {
              digitalWrite(ledPin1, LOW);
              digitalWrite(ledPin2, HIGH);
              time1 = micros();
              //Serial.println("EVEN");
              evenNumber = HIGH;
          }
          button3Pressed = LOW;
          
      }

      reading3 = digitalRead(buttonPin3);         //read button3
      
      if(takeReading == HIGH)
      {
          if(oddNumber == HIGH)
              reading1 = digitalRead(buttonPin1);
          if(evenNumber == HIGH)    
              reading2 = digitalRead(buttonPin2);
      } 
    
      // If the switch changed, due to noise or pressing:
      if (reading1 != lastButtonState1 || reading2 != lastButtonState2 || reading3 != lastButtonState3) 
      {
        // reset the debouncing timer
        lastDebounceTime = micros();
      }
      
      if ((micros() - lastDebounceTime) > debounceDelay) 
      {
          // whatever the reading is at, it's been there for longer
          // than the debounce delay, so take it as the actual current state:
          // if the button state has changed:
          if (reading1 != buttonState1)
          {
                buttonState1 = reading1;
                
                // only toggle the LED if the new button state is HIGH
                if (buttonState1 == HIGH) 
                {
                  Serial.print("Your reaction time is: ");
                  Serial.print(lastDebounceTime - time1);
                  Serial.println("us");
                  Serial.println();
                  printcount = 0;
                }
          }

          if (reading2 != buttonState2)
          {
                buttonState2 = reading2;
                
                // only toggle the LED if the new button state is HIGH
                if (buttonState2 == HIGH) 
                {
                  Serial.print("Your reaction time is: ");
                  Serial.print(lastDebounceTime - time1);
                  Serial.println("us");
                  Serial.println();
                  printcount = 0;
                }
          }

          if (reading3 != buttonState3)
          {
                buttonState3 = reading3;
                
                // only toggle the LED if the new button state is HIGH
                if (buttonState3 == HIGH) 
                {
                  Serial.println("READY TO GO");
                  button3Pressed = HIGH;
                  takeReading = HIGH;
                }
          }
      }
    
      // save the reading.  Next time through the loop,
      // it'll be the lastButtonState:
      lastButtonState1 = reading1;
      lastButtonState2 = reading2;
      lastButtonState3 = reading3;

}

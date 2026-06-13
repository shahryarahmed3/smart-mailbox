// Arduino 1 -->

#include <LiquidCrystal.h>


const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


const int buttonPin = 6;
int buttonState = 0;     // Current state of the button
int oldButtonState = LOW;
char inByte = '0';

void setup(){
 lcd.begin(16,2);
 pinMode(buttonPin, INPUT_PULLUP);
 //pinMode(ledPin, OUTPUT);
 Serial.begin(9600);
}

void loop(){
  
  //lcd.print("Hello");


  // Sending command: 
  // '4' to unlock (sent to Arduino 3)
  buttonState = digitalRead(buttonPin);

  if (buttonState != oldButtonState &&
      buttonState == HIGH)
  { 
    if (buttonState == LOW){
      //Serial.write('5');
    }
    else if (buttonState == HIGH){
      Serial.write('4');
 
    }
    delay(50);
  }
  
  oldButtonState = buttonState;
  
  
  //
  // Recieving info: 
  // '1' if mail
  // '0' if no mail
  if(Serial.available())
  {
    inByte = Serial.read();
    
    if(inByte == '1')
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("You have mail");
    }
    else if(inByte == '0')
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("No mail");
    }
    
    //
    // Recieving info: 
  	// '2' if door closed
  	// '3' if door opened
    if (inByte == '2')
    {
      Serial.println("Door is closed");
      lcd.setCursor(0,1);
      lcd.print("Door is closed");
    }
    else if (inByte == '3')
    {
      Serial.println("Door is open");
      lcd.setCursor(0,1);
      lcd.print("Door is open");
    }
  }

  delay(10);
 
}

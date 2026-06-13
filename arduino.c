/*Project Description: 
The Smart Mailbox system employs three Arduino devices to ensure secure access to a mailbox 
and offers real-time notifications about the presence of new mail. The project includes a default pin
that users can choose to share or keep private. When accessed safely, an LCD display notifies the user 
about the mailbox access or the arrival of new mail. This notification also occurs if the user has previously 
unlocked their mailbox. Manual lock and unlock functions can be controlled through pushbuttons connected to one of the Arduinos.
In case of unauthorized access, the LCD screen displays a warning message to alert the user.
*/

// Arduino 3 --->

// References: https://www.circuitbasics.com/how-to-set-up-a-keypad-on-an-arduino/
// https://wiki-content.arduino.cc/en/Tutorial/LibraryExamples/Sweep
#include <Keypad.h>
#include <Servo.h> // for servo


unsigned long ledActivationTime = 0;
const unsigned long ledDuration = 5000;

Servo myservo;
int pos = 0; // stores servo position

const byte ROWS = 4;
const byte COLS = 4;

// char hexaKeys[ROWS][COLS] = {
//   {'1', '2', '3', 'A'},
//   {'4', '5', '6', 'B'},
//   {'7', '8', '9', 'C'},
//   {'*', '0', '#', 'D'}
// };
char hexaKeys[ROWS][COLS] = {
  {'D', '#', '0', '*'},
  {'C', '9', '8', '7'},
  {'B', '6', '5', '4'},
  {'A', '3', '2', '1'}
};

const int ledPin = 10; // Substituting for lock solenoid

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Set the 4-digit password
char password[] = {'1', '2', '3', '4'};
char enteredPassword[4];
int passwordIndex = 0;

char inByte = '0';

void setup() {
  pinMode(ledPin, OUTPUT);
  myservo.attach(11);
  Serial.begin(9600);
}

void loop() {
 
  // After ledDuratation (seconds) turn the LED off. 
  if (millis() - ledActivationTime >= ledDuration && digitalRead(ledPin) == HIGH) {
    digitalWrite(ledPin, LOW);  // Turn off the LED
  }
  
  // Listening on Serial for unlock command
  // from the House Controller (Arduino 1)
  if (Serial.available()){
    inByte = Serial.read();
    
    if (inByte == '4'){
      servoSweep();
      turnOnLEDFor5Seconds();
    }
    
  }
  
    
  // KEYPAD LOGIC
  char customKey = customKeypad.getKey();

  if (customKey) {
    Serial.print("Entered: ");
    Serial.println(customKey);

    // Check if the entered key is a digit
    if (isdigit(customKey)) {
      // Store the entered digit in the password array
      enteredPassword[passwordIndex] = customKey;
      passwordIndex++;

      // If the entire password is entered, check it
      if (passwordIndex == 4) {
        checkPassword();
      }
    } else {
      // Reset the password if a non-digit key is pressed
      resetPassword();
    }
  }
}

void checkPassword() {
  // Compare the entered password with the correct password
  if (strncmp(enteredPassword, password, 4) == 0) {
    Serial.println("Correct Password! Access Granted");
    servoSweep();
    turnOnLEDFor5Seconds();
  } else {
    Serial.println("Incorrect Password! Access Denied");
    digitalWrite(ledPin, LOW);   // Turn off an LED or perform desired action
  }

  // Reset the entered password
  resetPassword();
}

void resetPassword() {
  memset(enteredPassword, 0, sizeof(enteredPassword));
  passwordIndex = 0;
}


void turnOnLEDFor5Seconds() {
  digitalWrite(ledPin, HIGH);  // Turn on the LED
  ledActivationTime = millis();  // Record the current time
}

void servoSweep(){
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

// Arduino 2 -->

// Reference: https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/

const int trigPin = 3; // trig 
const int echoPin = 2; // echo
long duration;
int distance;
double distanceInch;

const int buttonPin = 4; // subsitute for door sensor

const int magnet_switch = 8; 

const int ledPin = 13;   // the number of the LED pin
int ledState = LOW;      // the current state of the output pin
int buttonState = 0;     // Current state of the button


void setup(){
 // Trig Pin
 pinMode(trigPin, OUTPUT);
 // Echo Pin
 pinMode(echoPin, INPUT);
  
 //pinMode(buttonPin, INPUT_PULLUP);
 pinMode(magnet_switch, INPUT_PULLUP);
 pinMode(ledPin, OUTPUT);
 Serial.begin(9600);
}

double getDistance(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  //distance = duration * 0.034 / 2; //centimeter
  distanceInch = duration * 0.0133 / 2; //inch

  return distanceInch;
}

void loop(){
  
  //
  // Sends info:
  // '0' if no mail
  // '1' if mail
  if(getDistance() < 4.0){
    Serial.write('1');
  }
  else{
    Serial.write('0');
  }
  
  //
  // Sends info:
  // '2' if door closed
  // '3' if door open
  /*
  buttonState = digitalRead(buttonPin);
  if (buttonState == 0){
    Serial.write('2');
  }
  else if (buttonState == 1){
    Serial.write('3');
  }
  */
  
  // magneticswitch
  if (digitalRead(magnet_switch) == LOW) {
    Serial.println("Switch Closed");
    Serial.write('2');
    //digitalWrite(ledPin, HIGH);
    //while (digitalRead(magnet_switch) == LOW) {}
  }
  else {
    Serial.write('3');
    //digitalWrite(ledPin, LOW);
  }
  
  
  delay(500);
  

}


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
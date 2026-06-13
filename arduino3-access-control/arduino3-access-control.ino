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

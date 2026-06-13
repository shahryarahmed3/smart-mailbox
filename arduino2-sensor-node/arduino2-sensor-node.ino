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


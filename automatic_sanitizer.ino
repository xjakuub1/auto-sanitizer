#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Initialization of the servo motor
Servo Myservo;

// Definition of pins for the ultrasonic sensor
#define trigPin 10
#define echoPin 9

// Initialization of the LCD display (I2C address 0x27, 20 characters by 4 lines)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Variables for servo position, response time, and distance
int pos;
long response, distance;

// Constant for the display refresh rate
const long interval = 500;

// State variable for dispensing
bool dispensing = false;

void setup() {
  // Initialization of serial communication for debugging
  Serial.begin(9600);

  // Connecting the servo motor to pin 3
  Myservo.attach(3);

  // Initialization of the LCD display
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("Clean your hands");

  // Setting pins for the ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Trigger the trig pin to send an ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);   
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measuring the duration of the echo
  response = pulseIn(echoPin, HIGH);

  // Calculating distance in centimeters
  distance = response / 58.2;

  // Checking if the distance is less than 15 cm and greater than 0
  if (distance < 15 && distance > 0) {
    if (!dispensing) {
      // If not currently dispensing, display a message on the LCD
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Dispensing!");
      dispensing = true;
    }

    // Moving the servo motor to the 40-degree position and back
    for(pos = 0; pos <= 40; pos++) {
      Myservo.write(pos);
      delay(15);
    }
    for(pos = 40; pos >= 0; pos--) {
      Myservo.write(pos);
      delay(15);                                                 
    }
    delay(500); // Short pause after dispensing
  } else {
    if (dispensing) {
      // If the hand is out of range, display the default message on the LCD
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Clean your hands");
      dispensing = false;
    }
  }
  delay(10); // Short delay before the next measurement
}
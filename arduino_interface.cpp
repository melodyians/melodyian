#include <Arduino.h>
#include "melodyian.h"
#include "types.h"

namespace ArduinoInterface {
  // Pins
  const int redPin = 9; //LED 'Red' connected to pin 9
  const int greenPin = 11; //LED 'Green' connected to pin 10
  const int bluePin = 10; //LED 'Blue' connected to pin 11

  // Motor Pins
  const byte STBY = 4; //standby pin

  // Motor A
  const byte PWMA = 5; //Speed control of motor A
  const byte AIN1 = 3; //Direction pin
  const byte AIN2 = 2; //Direction pin

  // Motor B
  const byte PWMB = 6; //Speed control of motor B
  const byte BIN1 = 7; //Direction - moved to 3 from 6 
  const byte BIN2 = 8; //Direction pin

  // Tone and Amplitude
  const byte tonePin = 12;
  const byte ampSDNpin = 13;

  //const byte bat1MonPin = A0;    // input pin for battery 1 divider
  //byte bat2MonPin = 1;  //input pin for battery 2 divider


  void setupPins() {

     // LED Pins    
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    
    // Motor Pins
    pinMode(STBY, OUTPUT); 
    pinMode(PWMA, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(PWMB, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);

    pinMode(ampSDNpin, OUTPUT); //for turning on/off amp
    digitalWrite(ampSDNpin, LOW); //turn amp off at startup
  }


  /************* LED COLOR CONTROL FUNCTIONS ************/
  void writeToLED(byte red, byte green, byte blue)
  { 
    analogWrite(redPin, red);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);
  }

  void writeToLED(RGBColor color)
  { 
    analogWrite(redPin, color.r);
    analogWrite(greenPin, color.g);
    analogWrite(bluePin, color.b);
  }

  /**************DC MOTOR CONTROL FUNCTIONS***************/
  void moveMotor(int motor, int motorSpeed, int motorDirection)
  {
  //Move specific motor at speed and direction
  //motor: 1 for B, 0 for A
  //speed: 0 is off, and 255 is full speed
  //direction: 0 clockwise, 1 counter-clockwise

    digitalWrite(STBY, HIGH); //disable standby

    boolean inPin1 = LOW;
    boolean inPin2 = HIGH;

    if(motorDirection == 1)
    {
      inPin1 = HIGH;
      inPin2 = LOW;
    }

    if(motor == 0)
    {
      digitalWrite(AIN1, inPin1);
      digitalWrite(AIN2, inPin2);
      analogWrite(PWMA, motorSpeed);
    }
    else
    {
      digitalWrite(BIN1, inPin1);
      digitalWrite(BIN2, inPin2);
      analogWrite(PWMB, motorSpeed);
    }
  }

  void stopMotors()
  {
  //enable standby  
    digitalWrite(STBY, LOW); 
  }


  int readBattery()
  { 
    return analogRead(A0);
  }

  void playTone(unsigned int freq) {
    tone(tonePin, freq);
  }

  void toneOff() {
    noTone(tonePin);
  }

  bool ampIsOn() {
    return digitalRead(ampSDNpin) == HIGH;
  }

  void setAmpPower(bool on) {
    if (on) {
      digitalWrite(ampSDNpin, HIGH);
    } else {
      digitalWrite(ampSDNpin, LOW);
    }
  }

}

#include <Arduino.h>
#include "hardware.h"
#include "types.h"


ArduinoInterface::ArduinoInterface() {
  this->setupPins();
  this->stopMotors();
};


void ArduinoInterface::setupPins() {

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
void ArduinoInterface::writeToLED(byte red, byte green, byte blue)
{ 
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}


/**************DC MOTOR CONTROL FUNCTIONS***************/
void ArduinoInterface::moveMotor(int motor, int motorSpeed, int motorDirection)
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

void ArduinoInterface::stopMotors()
{
//enable standby  
  digitalWrite(STBY, LOW); 
}


int ArduinoInterface::readBattery()
{ 
  return analogRead(A0);
}

void ArduinoInterface::playTone(unsigned int freq) {
  tone(tonePin, freq);
}

void ArduinoInterface::toneOff() {
  noTone(tonePin);
}

bool ArduinoInterface::ampIsOn() {
  return digitalRead(ampSDNpin) == HIGH;
}

void ArduinoInterface::setAmpPower(bool on) {
  if (on) {
    digitalWrite(ampSDNpin, HIGH);
  } else {
    digitalWrite(ampSDNpin, LOW);
  }
}



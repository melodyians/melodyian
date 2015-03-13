#pragma once

#include <Arduino.h>
#include "types.h"


class HardwareInterface {

public:
  virtual void writeToLED(byte red, byte green, byte blue) = 0;

  virtual void moveMotor(int motor, int motorSpeed, int motorDirection) = 0;
  virtual void stopMotors() = 0;

  virtual int readBattery() = 0;

  virtual void playTone(unsigned int freq) = 0;
  virtual void toneOff() = 0;
  virtual bool ampIsOn() = 0;
  virtual void setAmpPower(bool on) = 0;
};


class ArduinoInterface : public HardwareInterface {

private:
  void setupPins();
  // Pins
  static const byte redPin = 9; //LED 'Red' connected to pin 9
  static const byte greenPin = 11; //LED 'Green' connected to pin 10
  static const byte bluePin = 10; //LED 'Blue' connected to pin 11

  // Motor Pins
  static const byte STBY = 4; //standby pin

  // Motor A
  static const byte PWMA = 5; //Speed control of motor A
  static const byte AIN1 = 3; //Direction pin
  static const byte AIN2 = 2; //Direction pin

  // Motor B
  static const byte PWMB = 6; //Speed control of motor B
  static const byte BIN1 = 7; //Direction - moved to 3 from 6 
  static const byte BIN2 = 8; //Direction pin

  // Tone and Amplitude
  static const byte tonePin = 12;
  static const byte ampSDNpin = 13;

  //const byte bat1MonPin = A0;    // input pin for battery 1 divider
  //byte bat2MonPin = 1;  //input pin for battery 2 divider


public:
  ArduinoInterface();

  void writeToLED(byte red, byte green, byte blue);
  void moveMotor(int motor, int motorSpeed, int motorDirection);
  void stopMotors();

  int readBattery();

  void playTone(unsigned int freq);
  void toneOff();
  bool ampIsOn();
  void setAmpPower(bool on);

};
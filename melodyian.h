#pragma once

#include "types.h"
#include "midicc.h"
#include "state.h"
#include "colorhelper.h"
#include "state.h"
#include "smoothing.h"


namespace Battery {
  void pingBatVoltage(bool midi_read);
}


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

class RobotOutput {

public:
  RobotOutput();
  byte r;
  byte g;
  byte b;
};


class Behavior {

protected:
    unsigned short timer;
    byte behavior_key;
    void setCurrentBehavior(byte behavior_key);
    void clearCurrentBehavior();
    void resetTimer();
    void incrementTimer(unsigned short dt);
    void decrementTimer(unsigned short dt);

public:
    Behavior();
    virtual void updateBehavior(unsigned short dt, RobotState * state, RobotOutput * output) = 0;
    virtual void updateBehaviorKey(byte control_number, byte value) = 0;
    byte getCurrentBehavior();

};


class LEDBehavior : public Behavior {

private:
  //RGBColor trans_color;
  boolean flashOnFlag;

  void flashBehavior(RobotState * state, RobotOutput * output);
  void fadeBehavior(RobotState * state, RobotOutput * output);
  void pulseBehavior(unsigned short dt, RobotState * state, RobotOutput * output);

public:
  LEDBehavior();
  void updateBehavior(unsigned short dt, RobotState * state, RobotOutput * output);
  void updateBehaviorKey(byte control_number, byte value);

};

class Robot {

public:
  Robot();
  RobotState * state;
  HardwareInterface * hardware;

  RobotOutput * output;
  LEDBehavior * led_behavior;
  
  void handleInput(byte control_number, byte value);
  void updateBehavior(unsigned short dt);
  void updateHardware();

};


namespace LED {
  void processLEDCC(byte channel, byte number, byte value);
  void updateLEDBehavior(RobotState * robot_state, HardwareInterface * hardware, unsigned short dt);
}


namespace Sound {
  void processSoundTriggers(unsigned long dt, RobotState * robot_state, HardwareInterface * hardware);
  void processSoundCC(byte channel, byte number, byte value);
}

namespace Motor {

  void processMotorCC (byte channel, byte number, byte value);
  void actuateMotors(HardwareInterface * hardware);
}

namespace Flags {
  bool melodyOneAct();
  bool melodyTwoAct();
  bool keyModeAct(); //*********added for new Flash when keyModeAct functionality********
  bool noteOn();
  bool MIDInotePanic(); //****added for new MIDI note stop command****

  void setMelodyOne(bool);
  void setMelodyTwo(bool);
  void setKeyMode(bool); //*********added for new Flash when keyModeAct functionality********
  void setNoteOn(bool);
  void setMIDInotePanic(bool); //****added for new MIDI note stop command****
}

namespace NoteControl {
  void noteOnControl(byte channel, byte note, byte velocity);
  void noteOffControl(byte channel, byte note, byte velocity);
  void allNoteOffControl();
  bool anyActingNotes();
  unsigned int currentHz();
}

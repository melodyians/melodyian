#pragma once

#include "types.h"
#include "midicc.h"
#include "state.h"
#include "colorhelper.h"

namespace Battery {
  void pingBatVoltage(bool midi_read);
}

namespace ArduinoInterface {

  void setupPins();

  void writeToLED(byte red, byte green, byte blue);
  void writeToLED(RGBColor color);

  void moveMotor(int motor, int motorSpeed, int motorDirection);
  void stopMotors();

  int readBattery();

  void playTone(unsigned int freq);
  void toneOff();
  bool ampIsOn();
  void setAmpPower(bool on);

}

namespace LED {
  void processLEDCC(byte channel, byte number, byte value);
  void updateLEDBehavior(RobotState * robot_state, unsigned long dt);
}


namespace Sound {
  void processSoundTriggers(unsigned long dt, RobotState * robot_state);
  void processSoundCC(byte channel, byte number, byte value);
}

namespace Motor {

  void processMotorCC (byte channel, byte number, byte value);
  void actuateMotors();
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

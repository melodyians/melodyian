#pragma once

#include "arduinotypes.h"
#include "midicc.h"

namespace Battery {
  void pingBatVoltage(bool midi_read);
}

namespace ArduinoPins {

  void setupPins();

  void writeToLED(byte red, byte green, byte blue);

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
  void readFromEEPROM();
  void writeEEPROMValues();
}


namespace Melodies {
  void processMelodyTriggers();
  void processMelodyCC(byte channel, byte number, byte value);
}

namespace Motor {

  void processMotorCC (byte channel, byte number, byte value);
  void actuateMotors();
}

namespace Flags {
  bool melodyOneAct();
  bool melodyTwoAct();
  bool noteOn();

  void setMelodyOne(bool);
  void setMelodyTwo(bool);
  void setNoteOn(bool);
}

namespace NoteControl {
  void noteOnControl(byte channel, byte note, byte velocity);
  void noteOffControl(byte channel, byte note, byte velocity);
  bool anyActingNotes();
  int lastNote();
}

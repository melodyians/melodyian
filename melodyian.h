#pragma once

// Allow libraries to use arduino type
typedef unsigned char byte;

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


namespace Flags {
  bool melodyOneAct();
  bool melodyTwoAct();
  bool noteOn();

  void setMelodyOne(bool);
  void setMelodyTwo(bool);
  void setNoteOn(bool);
}
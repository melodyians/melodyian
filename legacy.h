#include "types.h"
#include "core_interfaces.h"

namespace Battery {
  void pingBatVoltage(bool midi_read);
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

  void setMelodyOne(bool val);
  void setMelodyTwo(bool val);
  void setKeyMode(bool val); //*********added for new Flash when keyModeAct functionality********
  void setNoteOn(bool val);
  void setMIDInotePanic(bool val); //****added for new MIDI note stop command****
}

namespace NoteControl {
  void noteOnControl(byte channel, byte note, byte velocity);
  void noteOffControl(byte channel, byte note, byte velocity);
  void allNoteOffControl();
  bool anyActingNotes();
  unsigned int currentHz();
}

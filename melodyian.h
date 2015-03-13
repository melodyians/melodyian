#pragma once

#include "hardware.h"
#include "types.h"
#include "midicc.h"
#include "state.h"
#include "colorhelper.h"
#include "state.h"
#include "smoothing.h"


namespace Battery {
  void pingBatVoltage(bool midi_read);
}

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
  RGBColor transition_color;
  bool flashOnFlag;
  byte selected_light_preset;
  byte current_fade_preset;

  void triggerLightPreset(int preset_number, RobotState * state);
  void flashBehavior(RobotState * state, RobotOutput * output);
  void fadeBehavior(RobotState * state, RobotOutput * output);
  void pulseBehavior(unsigned short dt, RobotState * state, RobotOutput * output);

public:
  LEDBehavior();
  void updateBehavior(unsigned short dt, RobotState * state, RobotOutput * output);
  void updateBehaviorKey(byte control_number, byte value);
  void updateState(byte control_number, byte value, RobotState * state);

};

class Robot {

public:
  Robot(HardwareInterface * hardware);
  RobotState * state;
  HardwareInterface * hardware;

  RobotOutput * output;
  LEDBehavior * led_behavior;
  
  void handleInput(byte control_number, byte value);
  void updateBehavior(unsigned short dt);
  void updateHardware();

};


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

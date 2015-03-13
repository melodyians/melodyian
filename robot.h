#pragma once

#include "core_interfaces.h"
#include "types.h"

// LED stuff

class LEDBehavior : public Behavior {

private:
  RGBColor transition_color;
  bool flashOnFlag;
  byte selected_light_preset;
  byte current_fade_preset;
  float brightness;

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



// Main interface
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


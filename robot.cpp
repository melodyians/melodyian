#pragma once
#include "melodyian.h"

RobotOutput::RobotOutput() {
    r = 0;
    g = 0;
    b = 0;
}

Robot::Robot() {

  hardware = new ArduinoInterface();
  state = new RobotState();
  led_behavior = new LEDBehavior();
  output = new RobotOutput();

}


void Robot::handleInput(byte control_number, byte value) {
    state->updateInput(control_number, value);
    led_behavior->updateBehaviorKey(control_number, value);
}
  
void Robot::updateBehavior(unsigned short dt) {
    led_behavior->updateBehavior(dt, state, output);
}
  
void Robot::updateHardware() {
    hardware->writeToLED(output->r, output->g, output->b);
}

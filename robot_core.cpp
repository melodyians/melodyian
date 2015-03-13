#pragma once
#include "robot.h"


Robot::Robot(HardwareInterface * hw) {

  hardware = hw;
  state = new State();
  output = new Output();

  led_behavior = new RobotLEDBehavior();

}


void Robot::handleInput(byte control_number, byte value) {
    state->updateInput(control_number, value);
    led_behavior->updateState(control_number, value, state);
    led_behavior->updateBehaviorKey(control_number, value);
}
  
void Robot::updateBehavior(unsigned short dt) {
    led_behavior->updateBehavior(dt, state, output);
}
  
void Robot::updateHardware() {
    hardware->writeToLED(output->r, output->g, output->b);
}

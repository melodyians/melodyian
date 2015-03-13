#pragma once
#include "robot.h"


Robot::Robot(HardwareInterface * hw) {

  this->hardware = hw;
  this->state = new State();
  this->output = new Output();

  this->led_behavior = new RobotLEDBehavior();

}


void Robot::handleInput(byte control_number, byte value) {
    this->state->updateInput(control_number, value);
    this->led_behavior->updateState(control_number, value, state);
    this->led_behavior->updateBehaviorKey(control_number, value);
}
  
void Robot::updateBehavior(unsigned short dt) {
    this->led_behavior->updateBehavior(dt, state, output);
}
  
void Robot::updateHardware() {
    this->hardware->writeToLED(output->r, output->g, output->b);
}

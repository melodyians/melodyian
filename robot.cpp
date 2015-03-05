#pragma once
#include "melodyian.h"


Robot::Robot() {

  hardware = new ArduinoInterface();
  state = new RobotState();

}


void Robot::handleInput(byte control_number, byte value) {
    // TODO
    state->updateInput(control_number, value);
}
  
void Robot::updateBehavior(unsigned short dt) {

    // TODO
}
  
void Robot::updateHardware() {

    // TODO
}

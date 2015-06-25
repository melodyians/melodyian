#include "robot.h"
#include "helper_midi.h"


Robot::Robot(HardwareInterface * hw) {

  this->hardware = hw;
  this->state = new State();

  this->output = new Output();

  this->led_behavior = new RobotLEDBehavior();
  this->sound_behavior = new RobotSoundBehavior();
  this->motor_behavior = new RobotMotorBehavior();

}


void Robot::handleInput(byte control_number, byte value) {

    this->state->updateInput(control_number, value);

    this->led_behavior->updateState(control_number, value, state);
    this->sound_behavior->updateState(control_number, value, state);

    this->led_behavior->updateBehaviorKey(control_number, value);
    this->sound_behavior->updateBehaviorKey(control_number, value);
    this->motor_behavior->updateBehaviorKey(control_number, value);

}
  
void Robot::updateBehavior(unsigned short dt) {
  
    this->led_behavior->updateBehavior(dt, state, output);
    this->sound_behavior->updateBehavior(dt, state, output);
    this->motor_behavior->updateBehavior(dt, state, output);

}

void Robot::updateHardware() {
    
    // Light
    this->hardware->writeToLED(this->output->r, this->output->g, this->output->b);

    // Sound
    if (this->output->amp_on == true) {
        
        // Turn amp on if it is off
        if (!hardware->ampIsOn()) {
            this->hardware->setAmpPower(true);
        }

        this->hardware->playTone(this->output->tone);

    } else {
        
        // Turn amp off if it is on
        if (hardware->ampIsOn()) {
            this->hardware->setAmpPower(false);

        }
        this->hardware->toneOff();
    }

    // Motors
    if (output->motors_on) {
        this->hardware->moveMotor(0, output->motor_a.speed, output->motor_a.direction);
        this->hardware->moveMotor(0, output->motor_b.speed, output->motor_b.direction);
    } else {
        this->hardware->moveMotor(0, 0, output->motor_a.direction);
        this->hardware->moveMotor(0, 0, output->motor_b.direction);        
    }

}

void Robot::noteOnControl(byte channel, byte note, byte velocity) {
    MidiOut::WriteMidiNoteOn(note,  velocity);

    if (velocity > 0) {
        this->state->sound_state()->turnNoteOn(note);
    } else {
        this->state->sound_state()->turnNoteOff(note);
    }
}

void Robot::noteOffControl(byte channel, byte note, byte velocity) {
    MidiOut::WriteMidiNoteOff(note,  velocity);
    this->state->sound_state()->turnNoteOff(note);
}

#include "actions.h"
#include "state.h"
#include "types.h"
#include "smoothing.h"


RobotState::RobotState() {

    led_storage = new LEDStorage();
    led_storage->readFromEEPROM();
    
    // TODO must update this if we get a low value. Saves some meory.
    input_values = new InputValues(RED_CC);

    // input_values->storeInput(RATE1_CC, (byte) 84); // Default to 1000
}

void RobotState::updateInput(byte control_number, byte value) {
    input_values->storeInput(control_number, value);
}

void RobotState::setCurrentLEDValues(byte r, byte g, byte b) {
    input_values->storeInput(RED_CC, r);
    input_values->storeInput(GREEN_CC, g);
    input_values->storeInput(BLUE_CC, b);
}


// Wrapper to input values
bool RobotState::bypassRandomColor() {
    return Smoothing::booleanButton(input_values->getValue(COLORJITTERBYPASS_CC));
}

bool RobotState::bypassRandomNote() {
    return Smoothing::booleanButton(input_values->getValue(NOTEJITTERBYPASS_CC));
}

float RobotState::randomness() {
    return Smoothing::mapByteToPercentage(input_values->getValue(JITTER_CC));
}

unsigned int RobotState::decay() {
    return (unsigned int) input_values->getValue(FADESPD_CC);
}

int RobotState::rate() {
    return Smoothing::smoothRateFader(input_values->getValue(RATE1_CC));
}
    
byte RobotState::ledRedValue() {
    return Smoothing::smoothRBGFader(input_values->getValue(RED_CC));
}

byte RobotState::ledGreenValue() {
    return Smoothing::smoothRBGFader(input_values->getValue(GREEN_CC));
}

byte RobotState::ledBlueValue() {
    return Smoothing::smoothRBGFader(input_values->getValue(BLUE_CC));
}

byte RobotState::pulseValue() {
    return input_values->getValue(DYNAMIC_CC);
}

bool RobotState::colorOn() {
    return Smoothing::booleanButton(input_values->getValue(SETCOLQ_CC));
}

bool RobotState::recordEEPROMArmed() {
    return Smoothing::booleanButton(input_values->getValue(ARMEEPROM_CC));
}

void RobotState::disarmRecordEEPROM() {
    input_values->storeInput(ARMEEPROM_CC, (byte) 0);
}

bool RobotState::saveColorOn() {
    return Smoothing::booleanButton(input_values->getValue(WRITECOLOR_CC));
}

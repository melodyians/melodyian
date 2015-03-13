#include "core.h"
#include "types.h"

#include "constants_actions.h"
#include "helper_smoothing.h"


State::State() {

    led_storage = new LEDStorage();
    led_storage->readFromEEPROM();
    
    // TODO must update this if we get a low value. Saves some meory.
    input_values = new InputValues(RED_CC);

    // input_values->storeInput(RATE1_CC, (byte) 84); // Default to 1000
}

void State::updateInput(byte control_number, byte value) {
    input_values->storeInput(control_number, value);
}

void State::setCurrentLEDValues(byte r, byte g, byte b) {
    input_values->storeInput(RED_CC, r);
    input_values->storeInput(GREEN_CC, g);
    input_values->storeInput(BLUE_CC, b);
}


// Wrapper to input values
bool State::bypassRandomColor() {
    return Smoothing::booleanButton(input_values->getValue(COLORJITTERBYPASS_CC));
}

bool State::bypassRandomNote() {
    return Smoothing::booleanButton(input_values->getValue(NOTEJITTERBYPASS_CC));
}

float State::randomness() {
    return Smoothing::mapByteToPercentage(input_values->getValue(JITTER_CC));
}

unsigned int State::decay() {
    return (unsigned int) input_values->getValue(FADESPD_CC);
}

int State::rate() {
    return Smoothing::smoothRateFader(input_values->getValue(RATE1_CC));
}
    
byte State::ledRedValue() {
    return Smoothing::smoothRBGFader(input_values->getValue(RED_CC));
}

byte State::ledGreenValue() {
    return Smoothing::smoothRBGFader(input_values->getValue(GREEN_CC));
}

byte State::ledBlueValue() {
    return Smoothing::smoothRBGFader(input_values->getValue(BLUE_CC));
}

byte State::pulseValue() {
    return input_values->getValue(DYNAMIC_CC);
}

bool State::colorOn() {
    return Smoothing::booleanButton(input_values->getValue(SETCOLQ_CC));
}

bool State::recordEEPROMArmed() {
    return Smoothing::booleanButton(input_values->getValue(ARMEEPROM_CC));
}

void State::disarmRecordEEPROM() {
    input_values->storeInput(ARMEEPROM_CC, (byte) 0);
}

bool State::saveColorOn() {
    return Smoothing::booleanButton(input_values->getValue(WRITECOLOR_CC));
}

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

bool RobotState::saveColorOn() {
    return Smoothing::booleanButton(input_values->getValue(WRITECOLOR_CC));
}


/*
RobotState::RobotState(LEDStorage * storage) {

    led_storage = storage;

    transColor[0] = 0;
    transColor[1] = 0;
    transColor[2] = 0;

    initInputPtr<byte>(red_slider);
    initInputPtr<byte>(green_slider);
    initInputPtr<byte>(blue_slider);

    initInputPtr<int>(rate);
    *rate = 1000;

    initInputPtr<unsigned int>(decay);
    initInputPtr<float>(randomness);

    initInputPtr<bool>(bypass_random_color);
    initInputPtr<bool>(bypass_random_note);   


    // LED Presets
    initInputPtr<int>(lightPresetSelect);
    *lightPresetSelect = 1;

    initInputPtr<int>(activeLightPreset);
    *lightPresetSelect = 1;
    

    initInputPtr<bool>(arm_rec);
    initInputPtr<bool>(save_color); 

    initInputPtr<unsigned int>(color_pulse);
    initInputPtr<bool>(new_pulse); 


    initInputPtr<bool>(color_on); 
    initInputPtr<unsigned int>(current_light_behavior);


};


int* RobotState::getActiveLightPresetPtr() {

     if (*activeLightPreset > 8) {
        *activeLightPreset = 8;
     }
    
     return led_storage->getLightPresetPtr(*activeLightPreset); 
}

*/


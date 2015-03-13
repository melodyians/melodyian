#pragma once
#include "types.h"
#include "colorhelper.h"
#include "input.h"
#include "storage.h"
#include "types.h"
#include "midicc.h"

class RobotState {

private:
    InputValues * input_values;

public:
    RobotState();
    LEDStorage * led_storage; // TODO probably best private

    void updateInput(byte control_number, byte value);

    bool bypassRandomColor();
    bool bypassRandomNote();

    float randomness();
    unsigned int decay();
    int rate();
    
    byte ledRedValue();
    byte ledGreenValue();
    byte ledBlueValue();

    byte pulseValue();

    bool colorOn();

    bool recordEEPROMArmed();
    bool saveColorOn();
    void disarmRecordEEPROM();

    void setCurrentLEDValues(byte r, byte g, byte b);

};


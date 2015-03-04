#pragma once
#include "types.h"
#include "colorhelper.h"

template<typename T>
void initInputPtr(T * ptr) {
    ptr = new T;
    *ptr = 0;
}


class LEDStorage {

private:
    int * lightPresetData;

public:

    LEDStorage();

    int ** lightPresetArray;

    int* getLightPresetPtr(int i);

    void readFromEEPROM();
    void saveToEEPROM(int lightPresetVal);
};


class RobotState {


public:
    RobotState(LEDStorage * led_storage);

    RGBColor robot_led_color;
    LEDStorage * led_storage;

    byte * red_slider;
    byte * green_slider;
    byte * blue_slider;

    int * rate;
    unsigned int * decay;
    float * randomness;

    bool * bypass_random_color;
    bool * bypass_random_note;

    bool ** light_preset_buttons;

    // variable for storing last selected lightPreset value (1-8)
    int * lightPresetSelect;

    int * activeLightPreset;

    bool * arm_rec;
    bool * save_color; 

    int transColor[3];

    unsigned int * color_pulse;
    bool * new_pulse;

    int* getActiveLightPresetPtr();


    bool * color_on;
    unsigned int * current_light_behavior;


};

/*
class RobotBehavior {
    public:
        virtual void update(RobotState * robot_state, unsigned long dt) = 0;
}
*/



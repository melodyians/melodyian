#pragma once
#include "types.h"

template<typename T>
void initInputPtr(T * ptr) {
    ptr = new T;
    *ptr = 0;
}


class InputValues {

public:
    InputValues(int num_presets);

    byte * red_slider;
    byte * green_slider;
    byte * blue_slider;

    bool * set_color_button;
    bool * flash_button;
    bool * auto_fade_button;
    bool * dynamic_pulse_button;

    int * rate ;
    unsigned int * decay;
    float * randomness;

    bool * melody_one;
    bool * melody_two;

    bool * bypass_random_color;
    bool * bypass_random_note;

    bool * clear_light_preset;
    bool * save_light_preset;

    bool * arm_storage;

    bool ** light_preset_buttons;

};

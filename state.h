#pragma once
#include "types.h"

template<typename T>
void initInputPtr(T * ptr) {
    ptr = new T;
    *ptr = 0;
}


class RobotState {

public:
    RobotState(int num_presets);

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
    //bool * key_mode_act; //I didn't put this here originally, but the keyModeAct CC button seems to work just like melody_one and melody_two triggers...why?
    //bool * MIDI_Panic; //do I need this too?

    bool * bypass_random_color;
    bool * bypass_random_note;

    bool * clear_light_preset;
    bool * save_light_preset;

    bool * arm_storage;

    bool ** light_preset_buttons;

};

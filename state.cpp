#include "state.h"
#include "types.h"

RobotState::RobotState(int num_presets) {

    initInputPtr<byte>(red_slider);
    initInputPtr<byte>(green_slider);
    initInputPtr<byte>(blue_slider);

    initInputPtr<bool>(set_color_button);
    initInputPtr<bool>(flash_button);
    initInputPtr<bool>(auto_fade_button);
    initInputPtr<bool>(dynamic_pulse_button);      


    initInputPtr<int>(rate);
    initInputPtr<unsigned int>(decay);
    initInputPtr<float>(randomness);

    initInputPtr<bool>(melody_one);
    initInputPtr<bool>(melody_two);

    initInputPtr<bool>(melody_one); // is this necessary?
    //initInputPtr<bool>(key_mode_act); //does this need to be here?
    //initInputPtr<bool>(MIDI_Panic); //does this need to be here?

    initInputPtr<bool>(bypass_random_color);
    initInputPtr<bool>(bypass_random_note);   

    initInputPtr<bool>(save_light_preset);   
    initInputPtr<bool>(arm_storage);   

    light_preset_buttons = new bool*[num_presets];

    for (int i = 0; i < num_presets; i++) {
        initInputPtr<bool>(light_preset_buttons[i]);
    }

};

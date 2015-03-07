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

    void setCurrentLEDValues(byte r, byte g, byte b);

};

/*

  input_handler->registerInput(COLORJITTERBYPASS_CC, 
      new StoreMutatedInputValue<bool>(robot->state->bypass_random_color, Smoothing::booleanButton));
  
  input_handler->registerInput(NOTEJITTERBYPASS_CC, 
      new StoreMutatedInputValue<bool>(robot->state->bypass_random_note, Smoothing::booleanButton));

  input_handler->registerInput(JITTER_CC, 
      new StoreMutatedInputValue<float>(robot->state->randomness, Smoothing::mapByteToThousand));

  input_handler->registerInput(RED_CC, 
      new StoreMutatedInputValue<byte>(robot->state->red_slider, Smoothing::smoothRBGFader));

  input_handler->registerInput(GREEN_CC, 
      new StoreMutatedInputValue<byte>(robot->state->green_slider, Smoothing::smoothRBGFader));

  input_handler->registerInput(BLUE_CC, 
      new StoreMutatedInputValue<byte>(robot->state->blue_slider, Smoothing::smoothRBGFader));

  input_handler->registerInput(RATE1_CC, 
      new StoreMutatedInputValue<int>(robot->state->rate, Smoothing::smoothRateFader));

  // TODO: this seems to impact the rate sometimes.
  input_handler->registerInput(FADESPD_CC, 
      new StoreInputValue<unsigned int>(robot->state->decay));

  input_handler->registerInput(SETCOLQ_CC, 
      new StoreMutatedInputValue<bool>(robot->state->color_on, Smoothing::booleanButton));


  input_handler->registerInput(SETCOLQ_CC, 
      new StoreInputValue<unsigned int>(robot->state->current_light_behavior));

    */


  /*
    input_handler->registerInput(TRIGLP1_CC, new LightPresetCommand(robot_state, 1));
    input_handler->registerInput(TRIGLP2_CC, new LightPresetCommand(robot_state, 2));
    input_handler->registerInput(TRIGLP3_CC, new LightPresetCommand(robot_state, 3));
    input_handler->registerInput(TRIGLP4_CC, new LightPresetCommand(robot_state, 4));
    input_handler->registerInput(TRIGLP5_CC, new LightPresetCommand(robot_state, 5));
    input_handler->registerInput(TRIGLP6_CC, new LightPresetCommand(robot_state, 6));
    input_handler->registerInput(TRIGLP7_CC, new LightPresetCommand(robot_state, 7));
    input_handler->registerInput(TRIGLP8_CC, new LightPresetCommand(robot_state, 8));
  */
 

  /*
 
  input_handler->registerInput(DYNAMIC_CC, 
      new StoreMutatedInputValue<bool>(robot_state->new_pulse, Smoothing::booleanButton));
 
  input_handler->registerInput(DYNAMIC_CC, 
      new StoreInputValue<unsigned int>(robot_state->color_pulse));

  input_handler->registerInput(ARMEEPROM_CC, 
      new StoreMutatedInputValue<bool>(robot_state->arm_rec, Smoothing::booleanButton));

  input_handler->registerInput(WRITECOLOR_CC, 
      new StoreMutatedInputValue<bool>(robot_state->save_color, Smoothing::booleanButton));

  */


/*
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
*/

/*
class RobotBehavior {
    public:
        virtual void update(RobotState * robot_state, unsigned long dt) = 0;
}
*/



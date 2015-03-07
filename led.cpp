#include <Arduino.h>

#include "melodyian.h"
#include "easing.h"
#include "colorhelper.h"
#include "smoothing.h"


// TODO -- functions on output?
void setOuputColor(RobotOutput * output, byte r, byte g, byte b) {
  output->r = r;
  output->g = g;
  output->b = b;
}

void setOuputLEDBlack(RobotOutput * output) {
  output->r = 0;
  output->g = 0;
  output->b = 0;
}


void copyLEDStateToOutput(RobotState * state, RobotOutput * output) {
  output->r = state->ledRedValue();
  output->g = state->ledGreenValue();
  output->b = state->ledBlueValue();
}


LEDBehavior::LEDBehavior() 
  : Behavior()
{
  flashOnFlag = false;
  selected_light_preset = 1;
  current_fade_preset = 1;
}

void LEDBehavior::updateBehavior(unsigned short dt, RobotState * state, RobotOutput * output) {

  incrementTimer(dt);

  switch(behavior_key)
  {
    case(SETCOLQ_CC):
    {
      copyLEDStateToOutput(state, output); 
      break;
    }
    case(DYNAMICQ_CC):
    {
      pulseBehavior(dt, state, output);
      break;
    }
    case(FLASHQ_CC):
    {
      flashBehavior(state, output);
      break;
    }
    case(AUTOFADEQ_CC):
    {
      fadeBehavior(state, output);
      break;
    }
    default:
      {

        // TODO: More intelligent handling of buttons. 
        // Have priority over button states.
        if (state->colorOn()) {
          behavior_key = SETCOLQ_CC;
        } else {
          setOuputLEDBlack(output);
        }
      } 
  }

}

void LEDBehavior::triggerLightPreset(int preset_number, RobotState * state) {

    selected_light_preset = preset_number; 

    RGBColor preset_color = state->led_storage->getLightPresetColor(preset_number);
        
    if (state->saveColorOn()) {
      state->led_storage->setPresetColor(preset_number,
                                        state->ledRedValue(), 
                                        state->ledGreenValue(),
                                        state->ledBlueValue());
    } else {
      state->setCurrentLEDValues(preset_color.r, preset_color.g, preset_color.b);
    }
    
    transition_color = preset_color;

  }

void LEDBehavior::updateState(byte control_number, byte value, RobotState * state) {

  if (control_number == TRIGLP1_CC) { 
      triggerLightPreset(1, state);
    } else if (control_number == TRIGLP2_CC) {
      triggerLightPreset(2, state);
    } else if (control_number == TRIGLP3_CC) {
      triggerLightPreset(3, state);
    } else if (control_number == TRIGLP4_CC) {
      triggerLightPreset(4, state);
    } else if (control_number == TRIGLP5_CC) { 
      triggerLightPreset(5, state);
    } else if (control_number == TRIGLP6_CC) { 
      triggerLightPreset(6, state);
    } else if (control_number == TRIGLP7_CC) { 
      triggerLightPreset(7, state);
    } else if (control_number == TRIGLP8_CC) { 
      triggerLightPreset(8, state);
    } else if (control_number == AUTOFADEQ_CC && value != 127) {
    
    // This causes LED to always start at the last selected lightPreset color when starting up
    // the autoFade function. Otherwise the transColor never resets unless a new lightPreset 
    // is selected, which causes the LED color to always pickup where it left off when starting 
    // and stopping the autoFade sequence w/out selecting a new lightPreset.

    // Now, multiple robots being commanded to initiate the autoFade function at the same time 
    // should remain in sync and always start at the same lightPreset

    transition_color = state->led_storage->getLightPresetColor(selected_light_preset);
    current_fade_preset = selected_light_preset;
  }
}


void LEDBehavior::updateBehaviorKey(byte control_number, byte value) {

  // Standard behavior keys
  if (control_number == SETCOLQ_CC || 
      control_number == DYNAMICQ_CC || 
      control_number == FLASHQ_CC || 
      control_number == AUTOFADEQ_CC) {

    if (value == 127) {
      setCurrentBehavior(control_number);
    } else {
      clearCurrentBehavior();
    }

  }

  // When flash is triggered, turn the flag on.
  if (control_number == FLASHQ_CC && value != 127) {
    flashOnFlag = true;
  }
    
}


void LEDBehavior::flashBehavior(RobotState * state, RobotOutput * output) {
  float colorJitter = 0;

  if (state->bypassRandomColor() == false) {
      colorJitter = state->randomness();
  }

  // TODO -- port these to state
  if (Flags::melodyOneAct() || Flags::melodyTwoAct() || Flags::keyModeAct()) {

    // Flash only when the note is on.
    if (Flags::noteOn()) {
      // If colorJitter is 0 this just returns the color
      RGBColor adjusted_color = getRandomColor(colorJitter, 
                                               state->ledRedValue(),
                                               state->ledGreenValue(),
                                               state->ledBlueValue());
      setOuputColor(output, adjusted_color.r, adjusted_color.g, adjusted_color.b);
    } else {
      setOuputLEDBlack(output);
    }
  } else {

    float rate_interval = state->rate() / 8;

    // Check if we need to advance the state of the animation
    if (timer > rate_interval)
    { 
      RGBColor adjusted_color = getRandomColor(colorJitter, 
                                               state->ledRedValue(),
                                               state->ledGreenValue(),
                                               state->ledBlueValue());

      if (flashOnFlag == true) {
        setOuputColor(output, adjusted_color.r, adjusted_color.g, adjusted_color.b);
      } else {
        setOuputLEDBlack(output);
      }
      
      // Flip the on/off flag
      flashOnFlag = !flashOnFlag;
     
      // Reset the timer to time - how far we've come
      decrementTimer(rate_interval);
    }
  }

}

void LEDBehavior::fadeBehavior(RobotState * state, RobotOutput * output) {

  unsigned short fade_interval = state->rate() / 16;
  
  if (timer > fade_interval) {
    
    RGBColor next_preset_color = state->led_storage->getLightPresetColor(current_fade_preset);

    // If we have reached the target preset, move to next one.
    if (transition_color == next_preset_color) {
      current_fade_preset++;
    }

    // Loop Around
    if (current_fade_preset >= selected_light_preset + 1) {
      current_fade_preset = 1;
    }

    setOuputColor(output, transition_color.r, transition_color.g, transition_color.b);

    transition_color = crossFade(transition_color, next_preset_color);

    decrementTimer(fade_interval);
  }  

}

void LEDBehavior::pulseBehavior(unsigned short dt, RobotState * state, RobotOutput * output) {

  // If Arduino receives a DYNAMIC_CC MIDI message w/ value greater than 0, 
  // turn LED on using most recent color value and scale brightness based on CC value.
  // When Arduino receives a DYNAMIC_CC MIDI message w/ value == 0, start fading 
  // the LED brightness to 0 incrementally based on decat value

  float brightness;

  if (state->pulseValue() >= 1) {
    brightness = 0.01 * map(state->pulseValue(), 1, 127, 10, 100);
    if (brightness > 1.0) {
      brightness = 1.0;
    }
  } else {

    brightness = Smoothing::brightnessDecay(brightness, dt, state->decay());
  }     

  RGBColor color_buffer = colorWithAdjustedBrightness(state->ledRedValue(),
                                                      state->ledGreenValue(),
                                                      state->ledBlueValue(),
                                                      brightness);

  setOuputColor(output, color_buffer.r, color_buffer.g, color_buffer.b);

}

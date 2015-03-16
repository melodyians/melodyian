#include <Arduino.h>

#include "robot.h"
#include "legacy.h"

#include "constants_actions.h"

#include "helper_color.h"
#include "helper_smoothing.h"
#include "helper_midi.h"



RobotLEDBehavior::RobotLEDBehavior() : Behavior()
{
  brightness = 0.0f;
  flash_on_flag = false;
  selected_light_preset = 1;
  current_fade_preset = 1;
}

void SetOutputColorToState(Output * output, State * state) {
  output->setColor(state->ledRedValue(), state->ledGreenValue(), state->ledBlueValue());
}


void RobotLEDBehavior::updateBehavior(unsigned short dt, State * state, Output * output) {

  this->incrementTimer(dt);

  /*
  if (state->recordEEPROMArmed() && state->saveColorOn()) { 
    MidiCC::WriteMidiOut(99, state->ledRedValue());
    MidiCC::WriteMidiOut(99, state->ledGreenValue());
    MidiCC::WriteMidiOut(99, state->ledBlueValue());
    MidiCC::WriteMidiOut(99, selected_light_preset);
    state->led_storage->saveToEEPROM(selected_light_preset);
    state->disarmRecordEEPROM();
  } 
  */

  switch(behavior_key)
  {
    case(SETCOLQ_CC):
    {
      SetOutputColorToState(output, state); 
      break;
    }
    case(DYNAMICQ_CC):
    {
      this->pulseBehavior(dt, state, output);
      break;
    }
    case(FLASHQ_CC):
    {
      this->flashBehavior(state, output);
      break;
    }
    case(AUTOFADEQ_CC):
    {
      this->fadeBehavior(state, output);
      break;
    }
    default:
      {

        // TODO: More intelligent handling of buttons. 
        // Have priority over button states.
        if (state->colorOn()) {
          this->behavior_key = SETCOLQ_CC;
        } else {
          output->setColorBlack();
        }
      } 
  }

}

void RobotLEDBehavior::triggerLightPreset(int preset_number, State * state) {

    this->selected_light_preset = preset_number; 

    RGBColor preset_color = state->led_storage->getLightPresetColor(preset_number);
        
    if (state->saveColorOn()) {
      state->led_storage->setPresetColor(preset_number,
                                        state->ledRedValue(), 
                                        state->ledGreenValue(),
                                        state->ledBlueValue());
    } else {
      state->setCurrentLEDValues(preset_color.r, preset_color.g, preset_color.b);
    }
    
    this->transition_color = preset_color;

  }

void RobotLEDBehavior::updateState(byte control_number, byte value, State * state) {

  if (control_number == TRIGLP1_CC) {
      this->triggerLightPreset(1, state);
    } else if (control_number == TRIGLP2_CC) {
      this->triggerLightPreset(2, state);
    } else if (control_number == TRIGLP3_CC) {
      this->triggerLightPreset(3, state);
    } else if (control_number == TRIGLP4_CC) {
      this->triggerLightPreset(4, state);
    } else if (control_number == TRIGLP5_CC) { 
      this->triggerLightPreset(5, state);
    } else if (control_number == TRIGLP6_CC) { 
      this->triggerLightPreset(6, state);
    } else if (control_number == TRIGLP7_CC) { 
      this->triggerLightPreset(7, state);
    } else if (control_number == TRIGLP8_CC) { 
      this->triggerLightPreset(8, state);
    }


    if (control_number == AUTOFADEQ_CC && value != 127) {
    
    // This causes LED to always start at the last selected lightPreset color when starting up
    // the autoFade function. Otherwise the transColor never resets unless a new lightPreset 
    // is selected, which causes the LED color to always pickup where it left off when starting 
    // and stopping the autoFade sequence w/out selecting a new lightPreset.

    // Now, multiple robots being commanded to initiate the autoFade function at the same time 
    // should remain in sync and always start at the same lightPreset

    this->transition_color = state->led_storage->getLightPresetColor(selected_light_preset);
    this->current_fade_preset = selected_light_preset;
  }

  // TODO -- get a different CC for this
  if (control_number == WRITECOLOR_CC && value < 26) {
    state->setCurrentLEDValues(0, 0, 0);
  }
}


void RobotLEDBehavior::updateBehaviorKey(byte control_number, byte value) {

  // Standard behavior keys
  if (control_number == SETCOLQ_CC || 
      control_number == DYNAMICQ_CC || 
      control_number == FLASHQ_CC || 
      control_number == AUTOFADEQ_CC) {

    if (value == 127) {
      this->setCurrentBehavior(control_number);
    } else {
      this->clearCurrentBehavior();
    }

  }

  // When flash is triggered, turn the flag on.
  if (control_number == FLASHQ_CC && value != 127) {
    this->flash_on_flag = true;
  }
    
}


void RobotLEDBehavior::flashBehavior(State * state, Output * output) {
  float colorJitter = 0;

  if (state->bypassRandomColor() == false) {
      colorJitter = state->randomness();
  }

  // TODO -- port these to state
  if (Flags::melodyOneAct() || Flags::melodyTwoAct() || Flags::keyModeAct()) {

    // Flash only when the note is on.
    if (Flags::noteOn()) {
      // If colorJitter is 0 this just returns the color
      RGBColor adjusted_color = Color::GetRandomColor(colorJitter, 
                                               state->ledRedValue(),
                                               state->ledGreenValue(),
                                               state->ledBlueValue());
      output->setColor(adjusted_color.r, adjusted_color.g, adjusted_color.b);
    } else {
      output->setColorBlack();
    }
  } else {

    float rate_interval = state->rate() / 8;

    // Check if we need to advance the state of the animation
    if (timer > rate_interval)
    { 
      RGBColor adjusted_color = Color::GetRandomColor(colorJitter, 
                                               state->ledRedValue(),
                                               state->ledGreenValue(),
                                               state->ledBlueValue());

      if (flash_on_flag == true) {
        output->setColor(adjusted_color.r, adjusted_color.g, adjusted_color.b);
      } else {
        output->setColorBlack();
      }
      
      // Flip the on/off flag
      this->flash_on_flag = !flash_on_flag;
     
      // Reset the timer to time - how far we've come
      this->decrementTimer(rate_interval);
    }
  }

}

void RobotLEDBehavior::fadeBehavior(State * state, Output * output) {

  unsigned short fade_interval = state->rate() / 16;
  
  if (timer > fade_interval) {
    
    RGBColor next_preset_color = state->led_storage->getLightPresetColor(current_fade_preset);

    // If we have reached the target preset, move to next one.
    if (transition_color == next_preset_color) {
      this->current_fade_preset++;
    }

    // Loop Around
    if (current_fade_preset >= selected_light_preset + 1) {
      this->current_fade_preset = 1;
    }

    output->setColor(transition_color.r, transition_color.g, transition_color.b);

    this->transition_color = Color::CrossFade(transition_color, next_preset_color);

    this->decrementTimer(fade_interval);
  }  

}

void RobotLEDBehavior::pulseBehavior(unsigned short dt, State * state, Output * output) {

  // If Arduino receives a DYNAMIC_CC MIDI message w/ value greater than 0, 
  // turn LED on using most recent color value and scale brightness based on CC value.
  // When Arduino receives a DYNAMIC_CC MIDI message w/ value == 0, start fading 
  // the LED brightness to 0 incrementally based on decat value


  if (state->pulseValue() >= 1) {

    this->brightness = 0.01 * map(state->pulseValue(), 1, 127, 10, 100);
    
    if (brightness > 1.0) {
      this->brightness = 1.0;
    }
  
  } else {
    this->brightness = Smoothing::brightnessDecay(this->brightness, dt, state->decay());
  }     

  RGBColor color_buffer = Color::ColorWithAdjustedBrightness(state->ledRedValue(),
                                                      state->ledGreenValue(),
                                                      state->ledBlueValue(),
                                                      this->brightness);

  output->setColor(color_buffer.r, color_buffer.g, color_buffer.b);

}

#include <Arduino.h>

#include "melodyian.h"
#include "easing.h"
#include "colorhelper.h"
#include "smoothing.h"


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


void LEDBehavior::updateBehaviorKey(byte control_number, byte value) {

    if (control_number == SETCOLQ_CC)
    {
      if (value == 127) {
        setCurrentBehavior(control_number);
      } else {
        clearCurrentBehavior();
      }
    }

    if (control_number == DYNAMICQ_CC)
    {
      if (value == 127) {
        setCurrentBehavior(control_number);
      } else {
        clearCurrentBehavior();
      }  
    }

    if (control_number == FLASHQ_CC)
    {
      if (value == 127) {
        setCurrentBehavior(control_number);
      } else {
        flashOnFlag = true;
        clearCurrentBehavior();
       }
    }

    /*
    if (number == AUTOFADEQ_CC)
    {
      if (value == 127) {queue = number;}
      else
      {
        queue = 0;

        transColor[0] = getLightPresetPtr(lightPresetSelect)[0]; //this causes LED to always start at the last selected lightPreset color when starting up
        transColor[1] = getLightPresetPtr(lightPresetSelect)[1]; //the autoFade function. Otherwise the transColor never resets unless a new lightPreset is selected, which
        transColor[2] = getLightPresetPtr(lightPresetSelect)[2]; //causes the LED color to always pickup where it left off when starting and stopping the autoFade sequence w/out selecting a new lightPreset
        activeLightPreset = lightPresetSelect;                   //Now, multiple robots being commanded to initiate the autoFade function at the same time should remain in sync and always start at the same lightPreset

      }

    }
    */
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

namespace LED {


  float brightness = 1;

  //RGBColor myLEDColor;

  // Used to store CC value of 'DYNAMICQCC' (a light queue which will pulse the 
  // LED when receiving an external CC message derived from live audio.


  boolean lightOnState = false;
  boolean setColorAct = false;


  int queue; //the value of this variable determines which "light queues/patterns" are triggered in the loop() function below


  unsigned long currentMillis = 0;
  unsigned long previousMillis = 0;




  //================LED COLOR PRESET STORAGE & RECALL FUNCTIONS================


  /*
  void crossFade(int color1[3], int color2[3])
  {
    //writeToLED....SHOW color1 first!
    ArduinoInterface::writeToLED(color1[0], color1[1], color1[2]);
    
    //then calculate distance between color1 and color2  
    int stepR = Easing::calculateStep(color1[0], color2[0]);
    int stepG = Easing::calculateStep(color1[1], color2[1]); 
    int stepB = Easing::calculateStep(color1[2], color2[2]);
    
    //then increment color1 RGB values '1' closer to color2 values
    transColor[0] = Easing::calculateVal(stepR, color1[0]);
    transColor[1] = Easing::calculateVal(stepG, color1[1]);
    transColor[2] = Easing::calculateVal(stepB, color1[2]);
  }

  //current_color is transColor
  bool fadeTo(int current_color[3], int target_color[3])
  {  
    if( current_color[0] == target_color[0] &&
    current_color[1] == target_color[1] &&
    current_color[2] == target_color[2])
      return true;
    
    crossFade(current_color, target_color);
    return false;
  }


  void altFlash(RobotState * robot_state)
  {
    if (lightOnState == true) {
      robot_state->robot_led_color = colorWithAdjustedBrightness(robot_state->robot_led_color, brightness); 
      ArduinoInterface::writeToLED(robot_state->robot_led_color);
    } else {
      ArduinoInterface::writeToLED(0, 0, 0);
    }
    
    lightOnState = !lightOnState;
    
  }
  */


  /*

  void setRandomColor(float colorJitter, RobotState * robot_state) {
    RGBColor random_color = HSVtoRGB(random(0,360), 1, 1);
            
    float xr = colorJitter * random_color.r + ((1 - colorJitter) * *(robot_state->red_slider));
    float xg = colorJitter * random_color.g + ((1 - colorJitter) * *(robot_state->green_slider));
    float xb = colorJitter * random_color.b + ((1 - colorJitter) * *(robot_state->blue_slider));

    // excerpted from ministage code
    RGBColor temp_color = { 
                            max(min(xr,255),0), 
                            max(min(xg, 255),0), 
                            max(min(xb, 255), 0) 
                          }; 
    brightness = 1;

    robot_state->robot_led_color = colorWithAdjustedBrightness(temp_color, brightness); 
    ArduinoInterface::writeToLED(robot_state->robot_led_color);

  }

  void setLEDBlack() {
    ArduinoInterface::writeToLED(0, 0, 0);
  }
  */

  void updateLEDBehavior(RobotState * robot_state, HardwareInterface * hardware, unsigned short dt) {

    float colorJitter = 0;

    if (robot_state->bypassRandomColor() == false) {
        colorJitter = robot_state->randomness();
    }

    //===========LED LIGHT QUEUES==========   
    switch (queue)
    {   

      /*
      case FLASHQ_CC: //FLASH W/ colorJitter (randomness)...imported from ministage_complete_1_1 sketch.
      {
        if (Flags::melodyOneAct() || Flags::melodyTwoAct() || Flags::keyModeAct()) { //<-------********uncomment for new FLASH when keyModeAct functionality**************
          if (Flags::noteOn()) {

            if (*(robot_state->bypass_random_color)) {
                ArduinoInterface::writeToLED(*(robot_state->red_slider), 
                                     *(robot_state->green_slider), 
                                     *(robot_state->blue_slider));   

            } else {
              setRandomColor(colorJitter, robot_state);
            }
          } else {
            setLEDBlack();
          }
        } else {

          currentMillis = millis(); //get the current time
          unsigned int timeElapsed = currentMillis - previousMillis; //get how much time has passed since we updated the animation

          if(timeElapsed > (*(robot_state->rate) / 8)) //check if we need to advance the state of the animation
          { 
            RGBColor random_color = HSVtoRGB(random(0,360), 1, 1);

            float xr = colorJitter * random_color.r + ((1 - colorJitter) * *(robot_state->red_slider));
            float xg = colorJitter * random_color.g + ((1 - colorJitter) * *(robot_state->green_slider));
            float xb = colorJitter * random_color.b + ((1 - colorJitter) * *(robot_state->blue_slider));

            RGBColor temp_color = {max(min(xr,255),0) , max(min(xg, 255),0) , max(min(xb, 255), 0) }; //excerpted from ministage code
            brightness = 1;

            robot_state->robot_led_color = temp_color;

            altFlash(robot_state);
            //go to the first state
            previousMillis = currentMillis; //set the last time we advanced the state of the animation
          }
        } 
        break;
      }
        
        
      case AUTOFADEQ_CC: //Color Fade Over Time
      {
        currentMillis = millis(); //get the current time
        unsigned int timeElapsed = currentMillis - previousMillis; //get how much time has passed since we updated the animation
        
        if(timeElapsed > *(robot_state->rate) / 16) //rate1 represents how much time must pass between updates of the animation - we divide it by 16 to get it in a good range
        {
          if(activeLightPreset >= lightPresetSelect + 1)
          {
            activeLightPreset = 1;
          }
          
          if(fadeTo(transColor, getActiveLightPresetPtr()))
          {            
            activeLightPreset++;
          }

          previousMillis = currentMillis; //set the last time we advanced the state of the animation          
        }  
        break;
 
      }
      */       
        
     
      default:
      {
        // Removed
      } 
    }

  } 


  void processLEDCC(byte channel, byte number, byte value) 
  {

    //  QUEUE TOGGLE BUTTONS
    /*
    if (number == FLASHQ_CC)
    {
      if (value == 127) {
        queue = number;
      } else {
        lightOnState = true;
        queue = 0;
      }
    }

    if (number == AUTOFADEQ_CC)
    {
      if (value == 127) {queue = number;}
      else
      {
        queue = 0;

        transColor[0] = getLightPresetPtr(lightPresetSelect)[0]; //this causes LED to always start at the last selected lightPreset color when starting up
        transColor[1] = getLightPresetPtr(lightPresetSelect)[1]; //the autoFade function. Otherwise the transColor never resets unless a new lightPreset is selected, which
        transColor[2] = getLightPresetPtr(lightPresetSelect)[2]; //causes the LED color to always pickup where it left off when starting and stopping the autoFade sequence w/out selecting a new lightPreset
        activeLightPreset = lightPresetSelect;                   //Now, multiple robots being commanded to initiate the autoFade function at the same time should remain in sync and always start at the same lightPreset

      }

    }
    */

  }

}


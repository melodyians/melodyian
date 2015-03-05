#include <Arduino.h>

#include "melodyian.h"
#include "easing.h"
#include "colorhelper.h"

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
      case SETCOLQ_CC:
      {
        //COLOR SET AND MANUAL FADE
        hardware->writeToLED(robot_state->ledRedValue(),
                            robot_state->ledGreenValue(),
                            robot_state->ledBlueValue());               
        break;
      }

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
        
      /*
      case DYNAMICQ_CC: //Dynamic Pulse Control
      {                
        if (*(robot_state->new_pulse) == true && *(robot_state->color_pulse) >= 1) //if Arduino receives a DYNAMIC_CC MIDI message w/ value greater than 0, turn LED on using most recent color value and scale brightness based on CC value
        {
          brightness = 0.01 * map (*(robot_state->color_pulse), 1, 127, 10, 100);
          if (brightness > 1.0) {
            brightness = 1.0;
          }
        }

        else {

          brightness = Easing::brightnessDecay(brightness, dt, *(robot_state->decay));
        }  //when Arduino receives a DYNAMIC_CC MIDI message w/ value == 0, start fading the LED brightness to 0 incrementally based on fadeSpeed value     

        // Probably a common function
        robot_state->robot_led_color.r = *(robot_state->red_slider);
        robot_state->robot_led_color.g = *(robot_state->blue_slider);
        robot_state->robot_led_color.b = *(robot_state->green_slider);

        robot_state->robot_led_color = colorWithAdjustedBrightness(robot_state->robot_led_color, brightness);
        ArduinoInterface::writeToLED(robot_state->robot_led_color);

        break;
      }
      */
        
      default:
      {
        if (setColorAct == true) {
          queue = SETCOLQ_CC;
        } else {
          hardware->writeToLED(0, 0, 0);
        }
      } 
    }

  } 


  void processLEDCC(byte channel, byte number, byte value) 
  {

    //  QUEUE TOGGLE BUTTONS

    /*
    if (number == DYNAMICQ_CC)
    {
      if (value == 127) {
        queue = DYNAMICQ_CC;
      } else {
        queue = 0;
      }  
    }
    */
    
    if (number == SETCOLQ_CC) //'SET COLOR' Light queue toggle button
    {
      if (value == 127)
      {
        setColorAct = true;
        queue = number;
      }
      else
      {
        setColorAct = false;
        queue = 0;
      }
    }

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


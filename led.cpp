#include <Arduino.h>
#include <EEPROM.h>

#include "melodyian.h"
#include "easing.h"
#include "colorhelper.h"

using namespace ColorHelper;

namespace LED {

  byte fdr1; //currently used to store 'Red' value of LED
  byte fdr2; //currently used to store 'Green' value of LED
  byte fdr3; //currently used to store 'Blue' value of LED
  byte fdr4; //currently used to store CC value of 'DYNAMICQCC' (a light queue which will pulse the LED when receiving an external CC message derived from live audio. Not actively used right now, but will in the future. Ask Scott if you're curious for more details about this.

  boolean lightOnState = false;
  boolean setColorAct = false;

  boolean colorStoreReached = true;  //*****ONLY BEING USED IN INACTIVE fadeBetween() FUNCTION ******//
  int colorStore[3]; //*****ONLY BEING USED IN INACTIVE fadeBetween() FUNCTION ******//

  float brightness = 1;
  RGBColor myLEDColor;
  int queue; //the value of this variable determines which "light queues/patterns" are triggered in the loop() function below

  float xr;
  float xg;
  float xb;

  int transColor[3] = {0, 0, 0}; //starts off as 'black' (no light)
  int modWheel = 1; //**********THIS IS NOT BEING USED FOR ANYTHING RIGHT NOW*******//

  boolean armEEPROMwrite = false;
  // would like to rename, will eventually be a virtual button to toggle the arming of EEPROMwrite...
  boolean pShift = false; 


  unsigned long currentMillis = 0;
  unsigned long previousMillis = 0;


  // Formerly Rate1
  int colorRate = 1000;


  int lightPresetVal;
  int lightPresetData[24];
  int* lightPreset1 = lightPresetData; //0 1 2 lightPreset1[0] == lightPresetData[0], lightPreset1[3] == lightPreset2[0]
  int* lightPreset2 = lightPresetData + 3*1; //3 4 5
  int* lightPreset3 = lightPresetData + 3*2;
  int* lightPreset4 = lightPresetData + 3*3;
  int* lightPreset5 = lightPresetData + 3*4;
  int* lightPreset6 = lightPresetData + 3*5;
  int* lightPreset7 = lightPresetData + 3*6;
  int* lightPreset8 = lightPresetData + 3*7;

  int lightPresetSelect = 1; //variable for storing last selected lightPreset value (1-8)
  int fadeSpeed = 0;  //will be used by 'DYNAMICQCC' light pattern, but not actively used now. Ask Scott if you're curious for more details....
  float colorJitter = 0;  //variable used to randomize LED color

  int activeLightPreset = 1;

  int* getLightPresetPtr(int i)
  {
    return lightPreset1 + (i-1)*3;
  }

  int* getActiveLightPreset()
  {
     if(activeLightPreset > 8)
        activeLightPreset = 8;
    
     return getLightPresetPtr(activeLightPreset); 
  }


  //================LED COLOR PRESET STORAGE & RECALL FUNCTIONS================


  void crossFade(int color1[3], int color2[3])
  {
    //writeToLED....SHOW color1 first!
    ArduinoPins::writeToLED(color1[0], color1[1], color1[2]);
    
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


  /*
  void fadeBetween(int colorStore[3], int transColor[3], int color2[3]) // ***NOT USING THIS FUNCTION CURRENTLY...BUT MAY LIKE TO IN FUTURE** ORIGINAL fadeBetween function that works w/ lightPreset array input for third parameter
  {
   
   int thisColorStore = Color(colorStore[0], colorStore[1], colorStore[2]);
   int thisTransColor = Color(transColor[0], transColor[1], transColor[2]);
   int thisColor2 = Color(color2[0], color2[1], color2[2]);
   
   if (thisTransColor == thisColorStore) {colorStoreReached = true;}
   if (thisTransColor == thisColor2) {colorStoreReached = false;}
    
   if (colorStoreReached == true) {crossFade(transColor, color2);}
   else {crossFade(transColor, colorStore);}

  }
  */

  //vvvvvv=======Functions Matt wrote...used for fading from mini stage code========vvvvv

  RGBColor adjustBrightness(RGBColor color, float brightness)
  {
     if (brightness < 0) {
      brightness = 0;
     }

     byte r = color.r * brightness;
     byte g = color.g * brightness;
     byte b = color.b * brightness;

     RGBColor newcolor;
     newcolor.r = r;
     newcolor.g = g;
     newcolor.b = b;
     return newcolor;
  }

  float getBrightness(float Time, float ActivationTime, float FadeTime)
  {
    if(Time < ActivationTime)
      return 0.0f;
    if(Time > ActivationTime + FadeTime)
      return 0.0f;
    float b = 1 - ((Time - ActivationTime) / (FadeTime)); 
    if(b < 0)
      b = 0;
    return b;
  }

  float getActivateTime(int pixelIndex, float secondsPerCycle, int numberOfPixels)
  {  
    return pixelIndex * (secondsPerCycle / ((float)numberOfPixels));
  }


  /*
  void flash(byte redBrightness, byte greenBrightness, byte blueBrightness)
  {  
    if (lightOnState == true) {writeToLED(redBrightness, greenBrightness, blueBrightness);}  
    else {writeToLED(0, 0, 0);}
    
    lightOnState = !lightOnState;   
  }
  */


  #define MIN_FADE_TIME_TIMES_ONE_HUNDRED 10
  #define MAX_FADE_TIME_TIMES_ONE_HUNDRED 150

  void decay(int elapsed_millis, int fadeSpeed) {
    float delta_t = (elapsed_millis / 1000.0f) / (map(fadeSpeed,0,127,MIN_FADE_TIME_TIMES_ONE_HUNDRED,MAX_FADE_TIME_TIMES_ONE_HUNDRED) / 100.0f);
    brightness = brightness - delta_t;
  }





  void altFlash()
  {
    if (lightOnState == true) {
      myLEDColor = adjustBrightness(myLEDColor, brightness); 
      ArduinoPins::writeToLED(myLEDColor.r, myLEDColor.g, myLEDColor.b);
    } else {
      ArduinoPins::writeToLED(0, 0, 0);
    }
    
    lightOnState = !lightOnState;
    
  }

  void dynamicPulse()
  {       
      RGBColor thisLEDColor = adjustBrightness(myLEDColor, brightness);
      ArduinoPins::writeToLED(thisLEDColor.r, thisLEDColor.g, thisLEDColor.b);
  }

  void processLEDCC(byte channel, byte number, byte value) 
  {

    // ===EEPROM ARM BUTTON TOGGLE
    if (number == ARMEEPROM_CC)
    {
      if (value == 127) {
        armEEPROMwrite = true;
      }
      else {
        armEEPROMwrite = false;
      }
    }

    if (number == WRITECOLOR_CC) //=====USED TO BE PITCH SHIFT WHEEL WHEN WORKING W/ MINI STAGE=====
    {
      if (value > 120) {pShift = true;}
      else if (value < 26)
      {
       fdr1 = 0;
       fdr2 = 0;
       fdr3 = 0;
       pShift = false; 
      }
      else {pShift = false;}
    }

    //==========PRESSURE PADS======
    if (number == TRIGLP1_CC) 
    { 
      lightPresetVal = 1;
      lightPresetSelect = 1;
          
      if (pShift == false)
      {
      fdr1 = lightPreset1[0];
      fdr2 = lightPreset1[1];
      fdr3 = lightPreset1[2];
      }
      else
      {
      lightPreset1[0] = fdr1;
      lightPreset1[1] = fdr2;
      lightPreset1[2] = fdr3; 
      }
      
      transColor[0] = lightPreset1[0];
      transColor[1] = lightPreset1[1];
      transColor[2] = lightPreset1[2];
      
      colorStore[0] = lightPreset1[0];
      colorStore[1] = lightPreset1[1];
      colorStore[2] = lightPreset1[2];
    }
    
    if (number == TRIGLP2_CC) 
    { 
      lightPresetVal = 2;
      lightPresetSelect = 2;
       
      if (pShift == false)
      {
      fdr1 = lightPreset2[0];
      fdr2 = lightPreset2[1];
      fdr3 = lightPreset2[2];
      }
      else
      {
      lightPreset2[0] = fdr1;
      lightPreset2[1] = fdr2;
      lightPreset2[2] = fdr3; 
      }
      
      transColor[0] = lightPreset2[0];
      transColor[1] = lightPreset2[1];
      transColor[2] = lightPreset2[2];
      
      colorStore[0] = lightPreset2[0];
      colorStore[1] = lightPreset2[1];
      colorStore[2] = lightPreset2[2];
    }
    
    if (number == TRIGLP3_CC) 
    { 
      lightPresetVal = 3;
      lightPresetSelect = 3;
        
      if (pShift == false)
      {
      fdr1 = lightPreset3[0];
      fdr2 = lightPreset3[1];
      fdr3 = lightPreset3[2];
      }
      else
      {
      lightPreset3[0] = fdr1;
      lightPreset3[1] = fdr2;
      lightPreset3[2] = fdr3; 
      }
      
      transColor[0] = lightPreset3[0];
      transColor[1] = lightPreset3[1];
      transColor[2] = lightPreset3[2];
      
      colorStore[0] = lightPreset3[0];
      colorStore[1] = lightPreset3[1];
      colorStore[2] = lightPreset3[2];
    }
    
    if (number == TRIGLP4_CC) 
    { 
      lightPresetVal = 4;
      lightPresetSelect = 4;
          
      if (pShift == false)
      {
      fdr1 = lightPreset4[0];
      fdr2 = lightPreset4[1];
      fdr3 = lightPreset4[2];
      }
      else
      {
      lightPreset4[0] = fdr1;
      lightPreset4[1] = fdr2;
      lightPreset4[2] = fdr3; 
      }
      
      transColor[0] = lightPreset4[0];
      transColor[1] = lightPreset4[1];
      transColor[2] = lightPreset4[2];
      
      colorStore[0] = lightPreset4[0];
      colorStore[1] = lightPreset4[1];
      colorStore[2] = lightPreset4[2];
    }
    
    if (number == TRIGLP5_CC) 
    { 
      lightPresetVal = 5;
      lightPresetSelect = 5;
          
      if (pShift == false)
      {
      fdr1 = lightPreset5[0];
      fdr2 = lightPreset5[1];
      fdr3 = lightPreset5[2];
      }
      else
      {
      lightPreset5[0] = fdr1;
      lightPreset5[1] = fdr2;
      lightPreset5[2] = fdr3; 
      }
      
      transColor[0] = lightPreset5[0];
      transColor[1] = lightPreset5[1];
      transColor[2] = lightPreset5[2];
      
      colorStore[0] = lightPreset5[0];
      colorStore[1] = lightPreset5[1];
      colorStore[2] = lightPreset5[2];
    }
    
    if (number == TRIGLP6_CC)
    { 
      lightPresetVal = 6;
      lightPresetSelect = 6;
         
      if (pShift == false)
      {
      fdr1 = lightPreset6[0];
      fdr2 = lightPreset6[1];
      fdr3 = lightPreset6[2];
      }
      else
      {
      lightPreset6[0] = fdr1;
      lightPreset6[1] = fdr2;
      lightPreset6[2] = fdr3;
      }
      
      transColor[0] = lightPreset6[0];
      transColor[1] = lightPreset6[1];
      transColor[2] = lightPreset6[2];
      
      colorStore[0] = lightPreset6[0];
      colorStore[1] = lightPreset6[1];
      colorStore[2] = lightPreset6[2];
    }
    
    if (number == TRIGLP7_CC) 
    { 
      lightPresetVal = 7;
      lightPresetSelect = 7;
      
      if (pShift == false)
      {
      fdr1 = lightPreset7[0];
      fdr2 = lightPreset7[1];
      fdr3 = lightPreset7[2];
      }
      else
      {
      lightPreset7[0] = fdr1;
      lightPreset7[1] = fdr2;
      lightPreset7[2] = fdr3; 
      }
      
      transColor[0] = lightPreset7[0];
      transColor[1] = lightPreset7[1];
      transColor[2] = lightPreset7[2];
      
      colorStore[0] = lightPreset7[0];
      colorStore[1] = lightPreset7[1];
      colorStore[2] = lightPreset7[2];
    }
    
    if (number == TRIGLP8_CC) 
    { 
      lightPresetVal = 8;
      lightPresetSelect = 8;
          
      if (pShift == false)
      {
      fdr1 = lightPreset8[0];
      fdr2 = lightPreset8[1];
      fdr3 = lightPreset8[2];
      }
      else
      {
      lightPreset8[0] = fdr1;
      lightPreset8[1] = fdr2;
      lightPreset8[2] = fdr3; 
      }
      
      transColor[0] = lightPreset8[0];
      transColor[1] = lightPreset8[1];
      transColor[2] = lightPreset8[2];
      
      colorStore[0] = lightPreset8[0];
      colorStore[1] = lightPreset8[1];
      colorStore[2] = lightPreset8[2];
    }
    
    //==================LIGHT FADERS & QUEUE TOGGLE BUTTONS
    if (number == RED_CC)
    {
      fdr1 = Easing::smoothFade(value);
    }
    
    if (number == GREEN_CC)
    {
      fdr2 = Easing::smoothFade(value);
    }
    
    if (number == BLUE_CC)
    {
      fdr3 = Easing::smoothFade(value);
    }
    
    if (number == DYNAMIC_CC) //test signal for Dynamic Pulse light control
    {
      fdr4 = (value);
    }
    
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

    if (number == FLASHQ_CC)
    {
      if (value == 127) {queue = number;}
      
      else
      {
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

    if (number == DYNAMICQ_CC) //Light queue toggle buttons
    {
      if (value == 127) {queue = number;}
      else {queue = 0;}  
    }

      //============ ENCODERS ============
    
    if(number == RATE1_CC) //RATE #1  (knob pot 'B5' on axiom)
    {
      colorRate = Easing::mapRate1(value) * 2;
    }
    
    /*
    if (number == 76) //LIGHT PRESET SELECTOR (***REPLACE W/ RATE2 CONTROL, OR SOMETHING ELSE, OR DELETE OUTRIGHT***)
    { 
      byte thisValue = value + 1;
      lightPresetSelect = constrain(thisValue, 1, 8);
    }
    */
    
    if (number == FADESPD_CC) //PIXEL FADE/DECAY TIME
    {
      fadeSpeed = (value);
    } 
    
    if (number == JITTER_CC) //COLOR RANDOMNESS / JITTER CONTROL (knob B8)
    {
      colorJitter = map(value,0,127,0,1000)/1000.f;
    }


    //======Mod Wheel - Light Queue switcher
    
    if (number == 1) 
    {
     if (value >= 30 && value <= 115) {modWheel = 2;}
     else if (value >= 116) {modWheel = 3;}
     else {modWheel = 1;} 
    }

    //===========LED LIGHT QUEUES==========   
    switch (queue)
    {   
      case SETCOLQ_CC:  //COLOR SET AND MANUAL FADE
          ArduinoPins::writeToLED(fdr1, fdr2, fdr3);               
        break;
        
      case FLASHQ_CC: //FLASH W/ colorJitter (randomness)...imported from ministage_complete_1_1 sketch.
        {
          if (Flags::melodyOneAct()|| Flags::melodyOneAct())
          {
            if (Flags::noteOn())
            {
              RGBColor random_color = HSVtoRGB(random(0,360), 1, 1);
            
              float xr = colorJitter * random_color.r + ((1 - colorJitter) * fdr1);
              float xg = colorJitter * random_color.g + ((1 - colorJitter) * fdr2);
              float xb = colorJitter * random_color.b + ((1 - colorJitter) * fdr3);

              RGBColor temp_color = { max(min(xr,255),0), max(min(xg, 255),0), max(min(xb, 255), 0) }; //excerpted from ministage code
              brightness = 1;

              myLEDColor = adjustBrightness(temp_color, brightness); 
              ArduinoPins::writeToLED(myLEDColor.r, myLEDColor.g, myLEDColor.b);
            }

            else
            {
              ArduinoPins::writeToLED(0, 0, 0);
            }

          }

          else
          {
            currentMillis = millis(); //get the current time
            unsigned int timeElapsed = currentMillis - previousMillis; //get how much time has passed since we updated the animation
          
            if(timeElapsed > (colorRate / 8)) //check if we need to advance the state of the animation
            { 
              RGBColor random_color = HSVtoRGB(random(0,360), 1, 1);
            
              float xr = colorJitter * random_color.r + ((1 - colorJitter) * fdr1);
              float xg = colorJitter * random_color.g + ((1 - colorJitter) * fdr2);
              float xb = colorJitter * random_color.b + ((1 - colorJitter) * fdr3);
            
              RGBColor temp_color = {max(min(xr,255),0) , max(min(xg, 255),0) , max(min(xb, 255), 0) }; //excerpted from ministage code
              brightness = 1;

              myLEDColor = temp_color;
            
              altFlash();
            //go to the first state
              previousMillis = currentMillis; //set the last time we advanced the state of the animation
            }
          } 
        }
        break;
        
      case AUTOFADEQ_CC: //Color Fade Over Time
        {
          currentMillis = millis(); //get the current time
          unsigned int timeElapsed = currentMillis - previousMillis; //get how much time has passed since we updated the animation
          
          if(timeElapsed > colorRate / 16) //rate1 represents how much time must pass between updates of the animation - we divide it by 16 to get it in a good range
          {
            if(activeLightPreset >= lightPresetSelect + 1)
            {
              activeLightPreset = 1;
            }
            
            if(fadeTo(transColor, getActiveLightPreset()))
            {            
              activeLightPreset++;
            }
            //was working for 2 colors
            //fadeBetween(colorStore, transColor, lightPreset1); //original fadeBetween test
            previousMillis = currentMillis; //set the last time we advanced the state of the animation          
          }   
        }       
        break;
        
        
      case DYNAMICQ_CC: //Dynamic Pulse Control
        {        
          currentMillis = millis(); //get the current time
          unsigned int timeElapsedSinceLastDim = currentMillis - previousMillis;
               
          decay(timeElapsedSinceLastDim, fadeSpeed);    
          
          if (fdr4 >= 111) {brightness = 1;}
          if (fdr4 >= 95 && fdr4 <= 110) {brightness = .75;}               
          if (fdr4 >= 79 && fdr4 <= 94) {brightness = .6;}
          if (fdr4 >= 63 && fdr4 <= 78) {brightness = .5;}
          //if (fdr4 >= 55 && fdr4 <= 62) {brightness = .2;}
          //if (fdr4 >= 47 && fdr4 <= 54) {brightness = .1;}
          

          myLEDColor.r = fdr1;
          myLEDColor.g = fdr2;
          myLEDColor.b = fdr3;

          dynamicPulse();     
          previousMillis = currentMillis;
          
        }      
        break;
        
      default:
        if (setColorAct == true) {
          queue = SETCOLQ_CC;
        } else {
          ArduinoPins::writeToLED(0, 0, 0);
        } 
    }

  }


  // TODO: Refactor out
  void saveToEEPROM(int lightPresetVal)
  {
      int a = 0;
      int adrs = (lightPresetVal - 1) * 3;
      for (int i = adrs; i <= (adrs+2); i++)
      {
        EEPROM.write(i, getLightPresetPtr(lightPresetVal)[a]);
        a++;    
      } 
  }

  void readFromEEPROM()
  {  
    for (int i=0; i<=23; i++)
    { 
      lightPresetData[i] = EEPROM.read(i);
    }
  }

  void writeEEPROMValues() {

        //EEPROM Write functionality
    if (armEEPROMwrite == true && pShift == true) { 
        saveToEEPROM(lightPresetVal);
        armEEPROMwrite = false;
    }
  }

}


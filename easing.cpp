#include <Arduino.h>
#include "easing.h"

namespace Easing {

    byte smoothFade(byte value) //smoother fader mapping for RGB faders
    {
      int fdrval;
      if (value >= 0 && value <= 16)
        {fdrval = map(value, 0, 16, 0, 8);}
      else if (value >= 17 && value <= 100)
        {fdrval = map(value, 17, 100, 9, 200);}
      else {fdrval = map(value, 101, 127, 201, 255);}
      return fdrval;
    }

    byte motorFader(byte value)
    {
      byte fdrval;
      
      if (value >= 80)
        {fdrval = map(value, 80, 127, 1, 255);}
      else if (value <= 48)
        {fdrval = map(value, 48, 0, 1, 255);}
      else {fdrval = 0;}
      
      return fdrval;
    }


    #define MIN_FADE_TIME_TIMES_ONE_HUNDRED 10
    #define MAX_FADE_TIME_TIMES_ONE_HUNDRED 150

    float brightnessDecay(float brightness, int elapsed_millis, int fadeSpeed) {
      float delta_t = (elapsed_millis / 1000.0f) / (map(fadeSpeed,0,127,MIN_FADE_TIME_TIMES_ONE_HUNDRED,MAX_FADE_TIME_TIMES_ONE_HUNDRED) / 100.0f);
      return brightness - delta_t;
    }


    int calculateStep(int prevValue, int endValue)
    {
      int stepSize = endValue - prevValue;
      return stepSize;
    }

    int calculateVal(int stepSize, int val)
    {
      if (stepSize) // If step is non-zero
      { 
        if (stepSize > 0) {
          val += 1; //increment the value if step is positive... 
        } else if (stepSize < 0) {
          val -= 1;
        }  //...or decrement it if step is negative           
      }

      // Defensive driving: make sure val stays in the range 0-255
      if (val > 255) {
        val = 255;
      } else if (val < 0) {
        val = 0;
      }

      return val;
    }


    int mapRate1(int value) //Control for light speed / rate parameter settings - Knob B5
    {
      int encdrVal;
      
      if (value >= 0 && value <= 42) {
        encdrVal = map(value, 0, 42, 1, 100);
      } else if (value >= 43 && value <= 84) {
        encdrVal = map(value, 43, 84, 101, 1000);
      } else {
        encdrVal = map(value, 85, 127, 1001, 5000);
      }

      return encdrVal;
    }


    /*
    int mapRate2(int value) //Control for light speed / rate parameter settings - Knob B5
    {
      int encdrVal;
      if (value >= 0 && value <= 42) {encdrVal = map(value, 0, 42, 1, 100);}
      else if (value >= 43 && value <= 84) {encdrVal = map(value, 43, 84, 101, 1000);}
      else {encdrVal = map(value, 85, 127, 1001, 5000);}
      return encdrVal;
    }
    */

    /////////////UNUSED FADERS

      
      /*if (number == 26) //(D15 fader)
      { 
        fdr7 = (value); 
      }*/

     
      /*if (number == 28) //(D17 fader) stepper motor control fader - forward / backward
      {
        fdr9 = (value);
      }*/
  

}

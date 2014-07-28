#include <Arduino.h>
#include "smoothing.h"

namespace Smoothing {

    byte smoothRBGFader(byte value) //smoother fader mapping for RGB faders
    {
      int fdrval;
      if (value >= 0 && value <= 16) {
        fdrval = map(value, 0, 16, 0, 8);
      } else if (value >= 17 && value <= 100) {
        fdrval = map(value, 17, 100, 9, 200);
      } else {
        fdrval = map(value, 101, 127, 201, 255);
      }
      return fdrval;
    }

    byte smoothMotorFader(byte value)
    {
      byte fdrval;
      
      if (value >= 80) {
        fdrval = map(value, 80, 127, 1, 255);
      } else if (value <= 48) {
        fdrval = map(value, 48, 0, 1, 255);
      } else {
        fdrval = 0;
      }
      
      return fdrval;
    }

    int smoothRateFader(byte value) //Control for light speed / rate parameter settings - Knob B5
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

    bool booleanButton(byte value) {

      if (value == 127) {
        return true;
      } else {
        return false;
      }
    } 

}

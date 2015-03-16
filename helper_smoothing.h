#include "types.h"

namespace Smoothing {

  byte smoothRBGFader(byte value); // Smoother fader mapping for RGB faders
  byte mapByteToRGBFader(byte value); // Map output back to input value
  
  byte smoothMotorFader(byte value);

  int smoothRateFader(byte value); //Control for light speed / rate parameter settings - Knob B5

  bool booleanButton(byte value);

  float mapByteToPercentage(byte value);

  float brightnessDecay(float brightness, int elapsed_millis, int fadeSpeed);

  int calculateVal(int stepSize, int val);
  
}



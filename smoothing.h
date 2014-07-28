#include "types.h"

namespace Smoothing {

  byte smoothRBGFader(byte value); //smoother fader mapping for RGB faders
  byte smoothMotorFader(byte value);

  int smoothRateFader(byte value); //Control for light speed / rate parameter settings - Knob B5

  bool booleanButton(byte value);

}



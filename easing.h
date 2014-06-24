#pragma once
#include "melodyian.h"

namespace Easing {
  byte smoothFade(byte value); //smoother fader mapping for RGB faders
  byte motorFader(byte value);
  int calculateStep(int prevValue, int endValue);
  int calculateVal(int stepSize, int val);
  int mapRate1(int value); //Control for light speed / rate parameter settings - Knob B5
}




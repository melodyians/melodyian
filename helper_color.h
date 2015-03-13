#pragma once
#include "types.h"

//Convert a given HSV (Hue Saturation Value) to RGB(Red Green Blue) and set the led to the color
//  h is hue value, integer between 0 and 360
//  s is saturation value, double between 0 and 1
//  v is value, double between 0 and 1

// Create a 24 bit color value from R,G,B
RGBColor HSVtoRGB(int h, double s, double v);

RGBColor colorWithAdjustedBrightness(RGBColor color, float brightness);
RGBColor colorWithAdjustedBrightness(byte r, byte g, byte b, float brightness);
RGBColor getRandomColor(float colorJitter, byte r, byte g, byte b);

bool colorsEqual(RGBColor color1, RGBColor color2);
byte clampColor(float value);
RGBColor crossFade(RGBColor origin, RGBColor destination);
#include "colorhelper.h"

RGBColor HSVtoRGB(int h, double s, double v) {
  
  //this is the algorithm to convert from RGB to HSV
  double r=0; 
  double g=0; 
  double b=0;

  //double hf=h/60.0;

  int i=(int)floor(h/60.0);
  double f = h/60.0 - i;
  double pv = v * (1 - s);
  double qv = v * (1 - s*f);
  double tv = v * (1 - s * (1 - f));

  switch (i)
  {
    case 0: //rojo dominante
      r = v;
      g = tv;
      b = pv;
      break;
    case 1: //verde
      r = qv;
      g = v;
      b = pv;
      break;
    case 2: 
      r = pv;
      g = v;
      b = tv;
      break;
    case 3: //azul
      r = pv;
      g = qv;
      b = v;
      break;
    case 4:
      r = tv;
      g = pv;
      b = v;
      break;
    case 5: //rojo
      r = v;
      g = pv;
      b = qv;
      break;
  }

  //set each component to a integer value between 0 and 255
  byte red = (byte)constrain((int)255*r,0,255);
  byte green = (byte)constrain((int)255*g,0,255);
  byte blue = (byte)constrain((int)255*b,0,255);

  RGBColor c;
  c.r = red;
  c.g = green;
  c.b = blue;
  return c;

}


RGBColor colorWithAdjustedBrightness(RGBColor color, float brightness)
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


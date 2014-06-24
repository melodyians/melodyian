#pragma once

//Convert a given HSV (Hue Saturation Value) to RGB(Red Green Blue) and set the led to the color
//  h is hue value, integer between 0 and 360
//  s is saturation value, double between 0 and 1
//  v is value, double between 0 and 1

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

uint32_t HSVtoRGB(int h, double s, double v) {
  //this is the algorithm to convert from RGB to HSV
  double r=0; 
  double g=0; 
  double b=0;

  double hf=h/60.0;

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
  return Color(red,green,blue);
}


int r(uint32_t color) //I think this return the 'R' value from a 3 byte color value (uint32_t)
{
  return (color & 0xFF0000) >> 16;
}

int g(uint32_t color) //I think this return the 'G' value from a 3 byte color value (uint32_t)
{
  return (color & 0xFF00) >> 8;
}

int b(uint32_t color) //I think this return the 'B' value from a 3 byte color value (uint32_t)
{
  return (color & 0xFF);
}

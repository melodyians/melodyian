#include "helper_color.h"

namespace Color {

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
      case 0: // red dominante
        r = v;
        g = tv;
        b = pv;
        break;
      case 1: //green
        r = qv;
        g = v;
        b = pv;
        break;
      case 2: 
        r = pv;
        g = v;
        b = tv;
        break;
      case 3: //blue
        r = pv;
        g = qv;
        b = v;
        break;
      case 4:
        r = tv;
        g = pv;
        b = v;
        break;
      case 5: //red
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

  RGBColor ColorWithAdjustedBrightness(byte r, byte g, byte b, float brightness)
  {
     if (brightness < 0) {
      brightness = 0;
     }
     RGBColor newcolor;
     newcolor.r = r * brightness;
     newcolor.g = g * brightness;
     newcolor.b = b * brightness;
     return newcolor;
  }


  RGBColor ColorWithAdjustedBrightness(RGBColor color, float brightness)
  {
    return ColorWithAdjustedBrightness(color.r, color.g, color.b, brightness);
  }

  byte ClampColor(float value) {
    return max(min(value,255),0);
  }

  RGBColor GetRandomColor(float colorJitter, byte r, byte g, byte b) {

      RGBColor random_color = HSVtoRGB(random(0,360), 1, 1);

      // Combine a random color with  a pre-existing color.           
      float xr = colorJitter * random_color.r + ((1.0f - colorJitter) * r);
      float xg = colorJitter * random_color.g + ((1.0f - colorJitter) * g);
      float xb = colorJitter * random_color.b + ((1.0f - colorJitter) * b);

      RGBColor combined_color = {ClampColor(xr), ClampColor(xg), ClampColor(xb)};
      return combined_color;
  }

  byte StepColor(short distance, byte val) {
     
    // Increment the value if we want to go positive...
    if (distance > 0) {
      val += 1;  
    } else if (distance < 0) {
      val -= 1;
    }  //...or decrement it if we need to go negative           
    
    val = ClampColor(val);
    return val;
  }

  RGBColor CrossFade(RGBColor origin, RGBColor destination) {
    
    // Calculate distance between destination and origin  
    short distance_r = destination.r - origin.r;
    short distance_g = destination.g - origin.g;
    short distance_b = destination.b - origin.b;

    // Then increment Origin RGB values 1 closer to destination values
    RGBColor new_color = {
        StepColor(distance_r, origin.r),
        StepColor(distance_g, origin.g),
        StepColor(distance_b, origin.b)
    };

    return new_color;
  }

}

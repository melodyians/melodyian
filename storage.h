#pragma once
#include "colorhelper.h"

class LEDStorage {

private:
    int * lightPresetData;
    int ** lightPresetArray;
    int* getLightPresetPtr(int i);

public:

    LEDStorage();

    RGBColor getLightPresetColor(int i);
    void setPresetColor(int i, byte r, byte g, byte b);
    
    void readFromEEPROM();
    void saveToEEPROM(int lightPresetVal);
};
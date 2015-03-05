#pragma once

class LEDStorage {

private:
    int * lightPresetData;

public:

    LEDStorage();

    int ** lightPresetArray;

    int* getLightPresetPtr(int i);

    void readFromEEPROM();
    void saveToEEPROM(int lightPresetVal);
};
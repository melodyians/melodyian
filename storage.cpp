#import "storage.h"
#include <EEPROM.h>

LEDStorage::LEDStorage() {

    lightPresetData = new int[24];
    lightPresetArray = new int*[8];

    // Pointers to position in the int array.
    // Each pointer advances 3 indexes in the array, representing 3 bytes.
    int* lightPreset1 = lightPresetData;
    int* lightPreset2 = lightPresetData + 3*1; //3 4 5
    int* lightPreset3 = lightPresetData + 3*2;
    int* lightPreset4 = lightPresetData + 3*3;
    int* lightPreset5 = lightPresetData + 3*4;
    int* lightPreset6 = lightPresetData + 3*5;
    int* lightPreset7 = lightPresetData + 3*6;
    int* lightPreset8 = lightPresetData + 3*7;

    lightPresetArray[0] = lightPreset1;
    lightPresetArray[1] = lightPreset2;
    lightPresetArray[2] = lightPreset3;
    lightPresetArray[3] = lightPreset4;
    lightPresetArray[4] = lightPreset5;
    lightPresetArray[5] = lightPreset6;
    lightPresetArray[6] = lightPreset7;
    lightPresetArray[7] = lightPreset8;


}

void LEDStorage::saveToEEPROM(int lightPresetVal) {
    int a = 0;
    int adrs = (lightPresetVal - 1) * 3;
    for (int i = adrs; i <= (adrs+2); i++) {
      EEPROM.write(i, getLightPresetPtr(lightPresetVal)[a]);
      a++;    
    } 
}

void LEDStorage::readFromEEPROM() {  
    for (int i=0; i<=23; i++)
    { 
      lightPresetData[i] = EEPROM.read(i);
    }
}

int* LEDStorage::getLightPresetPtr(int i) {
    return lightPresetData + (i-1)*3;
}


RGBColor LEDStorage::getLightPresetColor(int i) {

    int * preset_pointer = getLightPresetPtr(i);
    RGBColor color = {
      preset_pointer[0],
      preset_pointer[1],
      preset_pointer[2]
    };
    return color;
}

void LEDStorage::setPresetColor(int i, byte r, byte g, byte b) {

    int * preset_pointer = getLightPresetPtr(i);

    preset_pointer[0] = r;
    preset_pointer[1] = g;
    preset_pointer[2] = b; 

}
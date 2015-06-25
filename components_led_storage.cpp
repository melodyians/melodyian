#import "components.h"
#include <EEPROM.h>

LEDStorage::LEDStorage() {

    this->lightPresetData = new int[24];
    this->lightPresetArray = new int*[8];

    // Create array out of initial preset pointers
    for (int i = 0; i < 8; i++) {
        this->lightPresetArray[i] = this->getLightPresetPtr(i + 1);
    }

}

int GetPointerIndex(int preset_num) {
    return (preset_num - 1) * 3;
}

void LEDStorage::saveToEEPROM(int preset_num) {

    RGBColor preset_color = this->getLightPresetColor(preset_num);

    int eeprom_byte = GetPointerIndex(preset_num);

    EEPROM.write(eeprom_byte, preset_color.r);
    EEPROM.write(eeprom_byte + 1, preset_color.g);
    EEPROM.write(eeprom_byte + 2, preset_color.b);

}

void LEDStorage::readFromEEPROM() {  
    for (int i=0; i<=23; i++) { 
      this->lightPresetData[i] = EEPROM.read(i);
    }
}

int* LEDStorage::getLightPresetPtr(int preset_num) {
    return this->lightPresetData + GetPointerIndex(preset_num);
}


RGBColor LEDStorage::getLightPresetColor(int preset_num) {

    int * preset_pointer = this->getLightPresetPtr(preset_num);
    RGBColor color = {
      preset_pointer[0],
      preset_pointer[1],
      preset_pointer[2]
    };
    return color;
}

void LEDStorage::setPresetColor(int preset_num, byte r, byte g, byte b) {

    int * preset_pointer = this->getLightPresetPtr(preset_num);

    preset_pointer[0] = r;
    preset_pointer[1] = g;
    preset_pointer[2] = b; 

}
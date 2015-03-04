#include "state.h"
#include "types.h"
#include <EEPROM.h>

RobotState::RobotState(LEDStorage * storage) {

    led_storage = storage;

    transColor[0] = 0;
    transColor[1] = 0;
    transColor[2] = 0;

    initInputPtr<byte>(red_slider);
    initInputPtr<byte>(green_slider);
    initInputPtr<byte>(blue_slider);

    initInputPtr<int>(rate);
    *rate = 1000;

    initInputPtr<unsigned int>(decay);
    initInputPtr<float>(randomness);

    initInputPtr<bool>(bypass_random_color);
    initInputPtr<bool>(bypass_random_note);   


    // LED Presets
    initInputPtr<int>(lightPresetSelect);
    *lightPresetSelect = 1;

    initInputPtr<int>(activeLightPreset);
    *lightPresetSelect = 1;
    

    initInputPtr<bool>(arm_rec);
    initInputPtr<bool>(save_color); 

    initInputPtr<unsigned int>(color_pulse);
    initInputPtr<bool>(new_pulse); 


    initInputPtr<bool>(color_on); 
    initInputPtr<unsigned int>(current_light_behavior);


};


int* RobotState::getActiveLightPresetPtr() {

     if (*activeLightPreset > 8) {
        *activeLightPreset = 8;
     }
    
     return led_storage->getLightPresetPtr(*activeLightPreset); 
}



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

void LEDStorage::saveToEEPROM(int lightPresetVal)
  {
      int a = 0;
      int adrs = (lightPresetVal - 1) * 3;
      for (int i = adrs; i <= (adrs+2); i++)
      {
        EEPROM.write(i, getLightPresetPtr(lightPresetVal)[a]);
        a++;    
      } 
  }

void LEDStorage::readFromEEPROM()
  {  
    for (int i=0; i<=23; i++)
    { 
      lightPresetData[i] = EEPROM.read(i);
    }
  }

/*
void LEDStorage::writeEEPROMValues() {

    if (armEEPROMwrite == true && writeColor == true) { 
        saveToEEPROM(lightPresetVal);
        armEEPROMwrite = false;
    }
}
*/


int* LEDStorage::getLightPresetPtr(int i) {
    return lightPresetData + (i-1)*3;
}


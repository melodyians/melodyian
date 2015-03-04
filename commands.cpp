#include "commands.h"


LightPresetCommand::LightPresetCommand(RobotState * state, int preset_num) {

    MidiCC::writeMidiCC(30,  1);
    this->preset_number = preset_num;

    MidiCC::writeMidiCC(30,  2);
    this->robot_state = state;
}

/*
void LightPresetCommand::readFromStorage(int* lightPresetPtr) {
      *(robot_state->red_slider) = lightPresetPtr[0];
      *(robot_state->green_slider) = lightPresetPtr[1];
      *(robot_state->blue_slider) = lightPresetPtr[2];
}

void LightPresetCommand::storeToStorage(int* lightPresetPtr) {

    lightPresetPtr[0] = *(robot_state->red_slider);
    lightPresetPtr[1] = *(robot_state->green_slider);
    lightPresetPtr[2] = *(robot_state->blue_slider); 
}
*/

void LightPresetCommand::execute(byte control_number, byte value) {

    /*
    int* lightPresetPtr = robot_state->led_storage->lightPresetArray[preset_number - 1];

    *(robot_state->lightPresetSelect) = preset_number;
        
    if (robot_state->save_color == false) {
        this->readFromStorage(lightPresetPtr);
    } else {
        this->storeToStorage(lightPresetPtr);
    }
    
    robot_state->transColor[0] = lightPresetPtr[0];
    robot_state->transColor[1] = lightPresetPtr[1];
    robot_state->transColor[2] = lightPresetPtr[2];
    */

}


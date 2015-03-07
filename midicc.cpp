#include <Arduino.h>

namespace MidiCC {

    void writeMidiOut(byte CC, byte value) {
      Serial.flush();
      byte MIDICCmessage[3] = {176, CC, value}; 
        
      Serial.write(MIDICCmessage, 3);
      
      //Serial.write(177); //Byte 1: message byte of MIDI CC message (1011CCCC... 10110001 = MIDI CC on channel 1)
      //Serial.write(CC); //Byte 2: Control number
      //Serial.write(value); //Byte 3: Value number
      
    }
}


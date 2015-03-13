#include <Arduino.h>
#include "helper_midi.h"

namespace MidiCC {

    void WriteMidiOut(byte CC, byte value) {
      Serial.flush();
      byte MIDICCmessage[3] = {176, CC, value}; 
        
      Serial.write(MIDICCmessage, 3);
      
    }
}


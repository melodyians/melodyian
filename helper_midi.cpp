#include <Arduino.h>
#include "helper_midi.h"

namespace MidiOut {

    void WriteMidiCC(byte CC, byte value) {
      Serial.flush();
      byte MIDIMessage[3] = {176, CC, value}; 
      Serial.write(MIDIMessage, 3); 
    }

    void WriteMidiNoteOn(byte note, byte velocity) {
      Serial.flush();
      byte MIDIMessage[3] = {144, note, velocity}; 
      Serial.write(MIDIMessage, 3); 
    }

    void WriteMidiNoteOff(byte note, byte velocity) {
      Serial.flush();
      byte MIDIMessage[3] = {128, note, velocity}; 
      Serial.write(MIDIMessage, 3); 
    }   
}


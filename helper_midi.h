#pragma once

#include "types.h"

namespace MidiOut {
    void WriteMidiCC(byte CC, byte value);
    void WriteMidiNoteOn(byte note, byte velocity);
    void WriteMidiNoteOff(byte note, byte velocity);

}




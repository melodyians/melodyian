#include "core_interfaces.h"
#include "helper_midi.h"

Behavior::Behavior() {
    clearCurrentBehavior();
    resetTimer();
}

void Behavior::setCurrentBehavior(byte key) { 
    behavior_key = key;
    resetTimer();
}

byte Behavior::getCurrentBehavior() {
    return behavior_key;
}

void Behavior::resetTimer() {
    timer = 0;
}

void Behavior::clearCurrentBehavior() {
    behavior_key = 0;
}

void Behavior::incrementTimer(unsigned short dt) {
    timer =  timer + dt;
}

void Behavior::decrementTimer(unsigned short dt) {
    timer =  timer - dt;
}


InputValues::InputValues(byte lowest_cc) {
    offset = lowest_cc;
    int array_size = (127 - lowest_cc) + 1;
    input_values = new byte[array_size];

    for (int i = 0; i < array_size; i++) {
        input_values[i] = (byte) 0;
    }
 
};

byte InputValues::getCCIndex(byte control_number) {
    return control_number - offset;
};

byte InputValues::getValue(byte control_number) {
    return input_values[getCCIndex(control_number)];
};

void InputValues::storeInput(byte control_number, byte value) {

    if (DEBUG) {
        MidiCC::WriteMidiOut(control_number,  value);
        MidiCC::WriteMidiOut(getCCIndex(control_number),  value);
    }

    input_values[getCCIndex(control_number)] = value;
};
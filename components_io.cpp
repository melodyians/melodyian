#include "components.h"
#include "helper_midi.h"

Output::Output() {
    r = 0;
    g = 0;
    b = 0;
}

// TODO -- functions on output?
void Output::setColor(byte r, byte g, byte b) {
  this->r = r;
  this->g = g;
  this->b = b;
}

void Output::setColorBlack() {
  this->r = 0;
  this->g = 0;
  this->b = 0;
}


InputValues::InputValues(byte lowest_cc) {
    this->offset = lowest_cc;
    int array_size = (127 - lowest_cc) + 1;
    this->input_values = new byte[array_size];

    for (int i = 0; i < array_size; i++) {
        this->input_values[i] = (byte) 0;
    }
 
};


byte InputValues::getCCIndex(byte control_number) {
    return control_number - this->offset;
};

byte InputValues::getValue(byte control_number) {
    return this->input_values[getCCIndex(control_number)];
};

void InputValues::storeInput(byte control_number, byte value) {

    if (DEBUG) {
        MidiOut::WriteMidiCC(control_number,  value);
        // MidiOut::WriteMidiCC(this->getCCIndex(control_number),  value);
    }

    this->input_values[getCCIndex(control_number)] = value;
};
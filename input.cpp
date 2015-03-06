#import "input.h"
#import "midicc.h"

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
        MidiCC::writeMidiCC(control_number,  value);
        MidiCC::writeMidiCC(getCCIndex(control_number),  value);
    }

    input_values[getCCIndex(control_number)] = value;
};


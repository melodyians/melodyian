#import "input.h"
#import "midicc.h"


InputValues::InputValues(byte lowest_cc) {
    offset = lowest_cc;
    input_values = new byte[(127 - lowest_cc) + 1];
};

byte InputValues::getCCIndex(byte control_number) {
    return control_number - offset;
};

byte InputValues::getValue(byte control_number) {
    return input_values[getCCIndex(control_number)];
};

void InputValues::storeInput(byte control_number, byte value) {
    MidiCC::writeMidiCC(control_number,  value);
    MidiCC::writeMidiCC(getCCIndex(control_number),  value);

    input_values[getCCIndex(control_number)] = value;
};


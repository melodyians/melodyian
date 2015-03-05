#pragma once;
#include "types.h"

class InputValues {

private:
    byte offset;
    byte * input_values;
    byte getCCIndex(byte control_number);

public:
    InputValues(byte lowest_cc);
    void storeInput(byte control_number, byte value);
    byte getValue(byte control_number);

};
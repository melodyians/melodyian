#pragma once
#include "types.h"
#include "midicc.h"
#include "state.h"

// Abstract parent function for commands. Command classes are initialized
// with a pointer to the state they alter.
class Command {
    public:
        virtual void execute(byte control_number, byte value) = 0;

};

template<typename T>
class StoreMutatedInputValue : public Command {

    private:
      // Pointer to the state value that will be changed by this command
      T * stored_var;

      // Function pointer to a function that alters the raw input value
      T (* valueMutator)(byte value);

    public:
    StoreMutatedInputValue(T * source, T (&vm)(byte)) {
        stored_var = source;
        valueMutator = vm;
    }

    void execute(byte control_number, byte value) {

        // Mutate the raw input value and store it in the pointer location
        (*stored_var) = valueMutator(value);

        // Write this out to serial for debugging
        MidiCC::writeMidiCC(control_number,  value);

    }
};

template<typename T>
class StoreInputValue : public Command {

    private:
      // Pointer to the state value that will be changed by this command
      T * stored_var;

    public:
    StoreInputValue(T * source) {
        stored_var = source;
    }

    void execute(byte control_number, byte value) {

        // Mutate the raw input value and store it in the pointer location
        *stored_var = (T) value;

        // Write this out to serial for debugging
        MidiCC::writeMidiCC(control_number,  value);

    }
};



class LightPresetCommand : public Command {

    private:
        //void readFromStorage(int* lightPresetPtr);
        //void storeToStorage(int* lightPresetPtr);

    public:
        RobotState * robot_state;
        int preset_number;
        LightPresetCommand(RobotState * state, int preset_num);
        void execute(byte control_number, byte value);
};

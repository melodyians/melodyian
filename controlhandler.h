#pragma once
#include "types.h"
#include "midicc.h"

class ControlChangeCommand {
    public:
        virtual void execute(byte control_number, byte value) = 0;

};

template<typename T>
class StoreInputValue : public ControlChangeCommand {

    private:
      T * stored_var;
      T (* valueMutator)(byte value);

    public:
    StoreInputValue(T * source, T (&vm)(byte)) {
        stored_var = source;
        valueMutator = vm;
    }

    void execute(byte control_number, byte value) {

        (*stored_var) = valueMutator(value);
        MidiCC::writeMidiCC(control_number,  (*stored_var));

    }
};

class HandlerList {
    public:
        HandlerList(ControlChangeCommand * c, HandlerList * n);
        ControlChangeCommand * command;
        HandlerList * next;
};


class ControlChangeHandler {

private:
    HandlerList ** handlers;

public:
    ControlChangeHandler(int size);
    void addCommand(byte control_number, ControlChangeCommand * command);
    void processMessage(byte channel, byte number, byte value);
};

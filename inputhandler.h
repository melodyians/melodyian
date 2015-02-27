#pragma once
#include "types.h"
#include "midicc.h"

// Command Pattern: http://gameprogrammingpatterns.com/command.html

// Abstract parent function for commands. Command classes are initialized
// with a pointer to the state they alter.
class Command {
    public:
        virtual void execute(byte control_number, byte value) = 0;

};

template<typename T>
class StoreInputValue : public Command {

    private:
      // Pointer to the state value that will be changed by this command
      T * stored_var;

      // Function pointer to a function that alters the raw input value
      T (* valueMutator)(byte value);

    public:
    StoreInputValue(T * source, T (&vm)(byte)) {
        stored_var = source;
        valueMutator = vm;
    }

    void execute(byte control_number, byte value) {

        // Mutate the raw input value and store it in the pointer location
        (*stored_var) = valueMutator(value);

        // Write this out to serial for debugging
        MidiCC::writeMidiCC(control_number,  (*stored_var));

    }
};

// Linked List containing 
class CommandList {
    public:
        CommandList(Command * c, CommandList * n);
        Command * command;
        CommandList * next;
};


class InputHandler {

private:
    CommandList ** commands;

public:
    InputHandler(int size);
    void registerInput(byte control_number, Command * command);
    void processInput(byte channel, byte number, byte value);
};

#pragma once
#include "types.h"
#include "commands.h"

// Command Pattern: http://gameprogrammingpatterns.com/command.html

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

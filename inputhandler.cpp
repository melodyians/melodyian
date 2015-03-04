#include <Arduino.h>
#include "inputhandler.h"
#include "types.h"

// Simple Linked List
CommandList::CommandList(Command * c, CommandList * n) {
    command = c;
    next = n;
}


// Control Change Handler
InputHandler::InputHandler(int size) {
    this->commands = new CommandList*[size];

    for (int i =0; i < size; i++) {
        this->commands[i] = 0;
    }
}

void InputHandler::registerInput(byte control_number, Command * command) {

    int cn = control_number;

    CommandList * this_command = new CommandList(command, NULL);

    if (this->commands[cn] != 0) {

        CommandList * list = commands[cn];

        while (list->next != NULL) {
            list = list->next;
        }

        list->next = this_command;

    } else {
        commands[cn] = this_command;
    }

};

void InputHandler::processInput(byte channel, byte control_number, byte value) {
      
    int cn = control_number;

    // If we have registered a handler for this MIDI control number
    if (this->commands[cn] != 0) {

        // Get the list of handlers we have registered for this number
        CommandList * list_node = this->commands[control_number];

        // Execute all of them
        do {
            list_node->command->execute(control_number, value);
            list_node = list_node->next;
        } while (list_node != NULL);

    }
    
  
};

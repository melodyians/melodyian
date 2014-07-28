#include <Arduino.h>
#include "controlhandler.h"
#include "types.h"

// Simple Linked List
HandlerList::HandlerList(ControlChangeCommand * c, HandlerList * n) {
    command = c;
    next = n;
}


// Control Change Handler
ControlChangeHandler::ControlChangeHandler(int size) {
    this->handlers = new HandlerList*[size];

    for (int i =0; i < size; i++) {
        this->handlers[i] = 0;
    }
}

void ControlChangeHandler::addCommand(byte control_number, ControlChangeCommand * command) {

    int cn = control_number;

    HandlerList * this_command = new HandlerList(command, NULL);

    if (this->handlers[cn] != 0) {

        HandlerList * list = handlers[cn];

        while (list->next != NULL) {
            list = list->next;
        }

        list->next = this_command;

    } else {
        handlers[cn] = this_command;
    }

};

void ControlChangeHandler::processMessage(byte channel, byte control_number, byte value) {
      
    int cn = control_number;


    if (this->handlers[cn] != 0) {

        HandlerList * list = this->handlers[control_number];

        do {
            list->command->execute(control_number, value);
        } while (list->next != NULL);

    }
    
  
};

#include "core_interfaces.h"


Behavior::Behavior() {
    clearCurrentBehavior();
    resetTimer();
}

void Behavior::setCurrentBehavior(byte key) { 
    behavior_key = key;
    resetTimer();
}

byte Behavior::getCurrentBehavior() {
    return behavior_key;
}

void Behavior::resetTimer() {
    timer = 0;
}

void Behavior::clearCurrentBehavior() {
    behavior_key = 0;
}

void Behavior::incrementTimer(unsigned short dt) {
    timer =  timer + dt;
}

void Behavior::decrementTimer(unsigned short dt) {
    timer =  timer - dt;
}

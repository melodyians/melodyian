#include "core.h"

Behavior::Behavior() {
    this->clearCurrentBehavior();
    this->resetTimer();
}

void Behavior::setCurrentBehavior(byte key) { 
    this->behavior_key = key;
    this->resetTimer();
}

byte Behavior::getCurrentBehavior() {
    return this->behavior_key;
}

void Behavior::resetTimer() {
    this->timer = 0;
}

void Behavior::clearCurrentBehavior() {
    this->behavior_key = 0;
}

void Behavior::incrementTimer(unsigned short dt) {
    this->timer =  this->timer + dt;
}

void Behavior::decrementTimer(unsigned short dt) {
    this->timer =  this->timer - dt;
}
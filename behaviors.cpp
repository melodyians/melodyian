#import "melodyian.h"


Behavior::Behavior() {
    clearCurrentBehavior();
    resetTimer();
}

void Behavior::setCurrentBehavior(byte key) { 
    behavior_key = key;
}

byte Behavior::getCurrentBehavior() {
    return behavior_key;
}

void Behavior::resetTimer() {
    currentMillis = 0;
    previousMillis = 0;
}

void Behavior::clearCurrentBehavior() {
    behavior_key = 0;
}




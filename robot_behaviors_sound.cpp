#include "robot.h"
#include "constants_actions.h"


RobotSoundBehavior::RobotSoundBehavior() : Behavior() {
// Foo
}

void RobotSoundBehavior::updateBehavior(unsigned short dt, State * state, Output * output) {

    switch(behavior_key)
    {
        case(KEYACT_CC):
        {
            if (state->sound_state()->anyActingNotes()) {
                output->amp_on = true;
                output->tone = state->sound_state()->currentHz();
            } else {
                output->amp_on = false;
            }
            break;
        }

        case(MIDIPANIC_CC):
        {
            state->sound_state()->turnOffAllNotes();
            output->amp_on = false;
            break;
        }
        default:
        {
            output->setAmpPower(false);
        }

    }


    // Foo
}

void RobotSoundBehavior::updateBehaviorKey(byte control_number, byte value) {

  // Sound behavior keys
  if (control_number == MEL1TRIG_CC || 
      control_number == MEL2TRIG_CC || 
      control_number == KEYACT_CC || 
      control_number == MIDIPANIC_CC) {

    if (value == 127) {
      this->setCurrentBehavior(control_number);
    } else {
      this->clearCurrentBehavior();
    }
  }

  if (control_number == MEL1TRIG_CC) {
    // Reset Melody to note 0
  } 

  if (control_number == MEL2TRIG_CC) {
    // Reset Melody to note 0
  }

}

void RobotSoundBehavior::updateState(byte control_number, byte value, State * state) {
    // TODO
}

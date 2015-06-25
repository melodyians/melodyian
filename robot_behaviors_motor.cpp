#include "robot.h"
#include "constants_actions.h"

RobotMotorBehavior::RobotMotorBehavior() : Behavior() {

}


bool HardLeft(int value) {
    return value < 5;
}

bool TurningLeft(int value) {
    return value >= 5 && value <= 58;
}

bool StraightAhead(int value) {
    return value >= 59 && value <= 69;
}

bool TurningRight(int value) {
    return value >= 70 && value <= 122;
}

bool HardRight(int value) {
    return value >= 123;
}


void RobotMotorBehavior::updateBehavior(unsigned short dt, State * state, Output * output) {

    switch(behavior_key)
    {
        // Only process input if Motor is On
        case(MTRACT_CC):
        {
            // Make sure motors are turned on
            output->motors_on = false;

            byte steering_direction = state->steerDirection();
            byte motor_speed = state->motorSpeedValue();
            // TODO -- some sort of state embedded in this being backwards

            if (StraightAhead(steering_direction)) {
                
                output->motor_a.direction = FORWARD;
                output->motor_b.direction = FORWARD;

                output->motor_a.speed = motor_speed;
                output->motor_b.speed = motor_speed;

            } else if (TurningRight(steering_direction)) {
                
                output->motor_a.direction = FORWARD;
                output->motor_b.direction = FORWARD;

                output->motor_a.speed = motor_speed;

                // Scale the speed of motor a depending on direction
                int scaling_value = map(steering_direction, 70, 122, 1, motor_speed);
                output->motor_b.speed = max(0, motor_speed - scaling_value);


            } else if (TurningLeft(steering_direction)) {

                output->motor_a.direction = FORWARD;
                output->motor_b.direction = FORWARD;

                output->motor_b.speed = motor_speed;

                // Scale the speed of motor b depending on direction
                int scaling_value = map(steering_direction, 58, 5, 1, motor_speed);
                output->motor_a.speed = max(0, motor_speed - scaling_value);

                
            } else if (HardRight(steering_direction)) {

                // One forward, one backward
                output->motor_a.direction = FORWARD;
                output->motor_b.direction = BACKWARD;

                // Ful speed
                output->motor_a.speed = motor_speed;
                output->motor_b.speed = motor_speed;
                
            } else if (HardLeft(steering_direction)) {

                // One forward, one backeward
                output->motor_a.direction = BACKWARD;
                output->motor_b.direction = FORWARD;

                // Full speed
                output->motor_a.speed = motor_speed;
                output->motor_b.speed = motor_speed;
   
            }

            // If our speed is reversed, swap direction
            /*
            if (motor_speed < 80) {
                output->reverseDirection();
            }
            */

            // If we are in reverse, swap directions.
            if (state->reverseOn()) {
                output->reverseDirection();
            }

            break;
        }
        default:
        {
            output->motors_on = false;
        }

    }

}

void RobotMotorBehavior::updateBehaviorKey(byte control_number, byte value) {

  // Sound behavior keys
  if (control_number == MTRACT_CC) {
    if (value == 127) {
      this->setCurrentBehavior(control_number);
    } else {
      this->clearCurrentBehavior();
    }
  }
}
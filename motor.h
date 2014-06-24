#pragma once

#include "midicc.h"
#include "arduinopins.h"

//=========MOTOR DRIVER SETUP============
//motor A connected between A01 and A02 on motor driver breakout
//motor B connected between B01 and B02 on motor driver breakout


int motorSpdVal = 0; //Master motor speed value
int steerDirection = 64; //steering direction value (0 = complete counter-clockwise rotation around center (both motors spinning in opposite directions at same speed), 64 = straight forwards (both motors spinning in same direction at same speed), 127 = complete clockwise rotation around center )

boolean motorAon = false;
boolean motorBon = false;

int motorAspeed = 0;
int motorBspeed = 0;

boolean motorAdirection = 0;
boolean motorBdirection = 0;



void steerControl(int value)
{
  /*
  Serial.print("MotorASpd: ");
  Serial.print(motorAspeed);
  Serial.print(" / MotorBSpd: ");
  Serial.print(motorBspeed);
  Serial.print(" / Steer Val: ");
  Serial.print(value);
  Serial.print(" / MtrADr: ");
  Serial.print(motorAdirection);
  Serial.print(" / MtrBDr: ");
  Serial.println(motorBdirection);
  */
  
 //Straight Ahead
    if ( value >= 59 && value <= 69)
    {
      motorAspeed = motorSpdVal;
      motorBspeed = motorSpdVal;
      motorAdirection = motorBdirection;
    }
    
    //Turning to the right
    else if (value >= 70 && value <= 122)
    {
     motorBdirection = motorAdirection;
     
     int rightWheelScalingRightT = map(value, 70, 122, 1, motorAspeed);
     
     motorAspeed = motorSpdVal;  //Left wheel
     
     motorBspeed = motorAspeed - rightWheelScalingRightT; //Right wheel...NOTE: may want to #define or create 'lowSpeedThres' variable for 4th parameter in preceding map function
     if (motorBspeed <= 0) {motorBspeed = 0;} //lowest speed control threshold, motorBspeed will never be less than 0     
    }
    
    //Hard right / Clockwise spinning
    else if (value >= 123)
    {
      motorAspeed = motorSpdVal;
      motorBdirection = !motorAdirection; //will this cause issues when trying to adjust the motorSpdVal while spinning? Try and find out...
      motorBspeed = motorSpdVal;      
    }
    
    //Turning to the left
    else if(value >= 5 && value <= 58)
    {
     motorAdirection = motorBdirection; 
      
     int leftWheelScalingLeftT = map(value, 58, 5, 1, motorBspeed);
      
     motorBspeed = motorSpdVal;  //Right wheel..NOT WORKING
     
     motorAspeed = motorBspeed - leftWheelScalingLeftT; //Left wheel...NOTE: may want to #define or create 'lowSpeedThres' variable for 4th parameter in preceding map function
     if (motorAspeed <= 0) {motorAspeed = 0;} //lowest speed control threshold, motorAspeed will never be less than 0 
    }
    
    //Hard left / Counter-Clockwise spinning
    else //(value <=4)
    {
      motorBspeed = motorSpdVal;
      motorAdirection = !motorBdirection; //will this cause issues when trying to adjust the motorSpdVal while spinning? Try and find out...
      motorAspeed = motorSpdVal;      
    } 
}

void processMotorCC (byte channel, byte number, byte value)
{
     //============MOTOR FADERS & TOGGLE BUTTONS
  
  if (number == MSTRMTRSPD_CC) //Master Motor Speed Control for new steering system (D16 fader)
  {
    motorSpdVal = motorFader(value);
    if (value >= 80)
    {
      motorAdirection = 0; //should be clockwise wheel rotation..
      motorBdirection = 0;  
    }
    else
    {
      motorAdirection = 1; //should be counterclockwise wheel rotation...
      motorBdirection = 1;
    }
  }
  
  if (number == MTRDIRREV_CC) //ALL MOTORS DIRECTION REVERSE TOGGLE
  {
      motorAdirection = !motorAdirection;
      motorBdirection = !motorBdirection;
  }
  
  if (number == MTRACT_CC) //MOTOR ACTIVATE TOGGLE (both motors on/off)
  {
    if (value == 127)
    {
      motorAon = true;
      motorBon = true;
    }
    else
    {
      motorAon = false;
      motorBon = false;
    }
  }
  
  if (number == MTRSTEER_CC) //"Steering Wheel" Control (Rotary encoder B1 pretending to be rotary pot)
  {
    steerDirection = value;    
  }
  
}


void processMotorMovement() {

  //============MOTOR CONTROLS===========
  steerControl(steerDirection);

  // MOTOR A
  if (motorAon == true) {
    moveMotor(0, motorAspeed, motorAdirection);
  } else {
    moveMotor(0, 0, motorAdirection);
  }
  // MOTOR B
  if (motorBon == true) {
    moveMotor(1, motorBspeed, motorBdirection);
  } else {
    moveMotor(1, 0, motorBdirection);
  }
  
}


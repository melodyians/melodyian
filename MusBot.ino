#include <EEPROM.h>
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
// #include <Wire.h>

#include "hardware_arduino.h"
#include "robot.h"

#include "constants_actions.h"

#include "helper_midi.h"
#include "helper_time.h"

#include "legacy.h"

MIDI_CREATE_DEFAULT_INSTANCE();

Timer * timer;
Robot * robot;

void setup()  {

  timer = new Timer();

  HardwareInterface * arduino_hardware = new ArduinoInterface();
  robot = new Robot(arduino_hardware);

  initializeInputs();
  
} 


void initializeInputs() {

  MIDI.begin();
  Serial.begin(57600);
  MIDI.turnThruOff();
  
  // Think I need to do this so Arduino can work w/ any MIDI message Max will be sending it
  MIDI.setInputChannel(MIDI_CHANNEL_OMNI); 
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandleNoteOn(noteOnControl);

  // Only needed if receiving MIDI Note Off messages from controller device to stop notes
  MIDI.setHandleNoteOff(noteOffControl); 

}


void loop()
{
  // Read midi from input. Triggers callbacks.
  bool midi_read = MIDI.read();

  // Move our clock forward
  unsigned short dt = timer->step();

  // Battery and Memory housekeeping
  // Disabled -- this seems to unpredictbly impact the state of the LED, and I have no idea why.
  // Battery::pingBatVoltage(midi_read, hardware_interface);
  
  // Update our Robot from our current state
  Sound::processSoundTriggers(dt, robot->state, robot->hardware);
  Motor::actuateMotors(robot->hardware);

  robot->updateBehavior(dt);
  robot->updateHardware();

  debug(dt);
}


unsigned short debug_every = 3000;
unsigned short debug_timer = 0;

void debug(unsigned short dt) {

    debug_timer = debug_timer + dt;
    if (debug_timer > debug_every) {
      debug_timer = 0;
      MidiCC::WriteMidiOut(RED_CC, robot->state->ledRedValue());
      MidiCC::WriteMidiOut(GREEN_CC, robot->state->ledGreenValue());
      MidiCC::WriteMidiOut(BLUE_CC, robot->state->ledBlueValue());
      MidiCC::WriteMidiOut(66, robot->led_behavior->getCurrentBehavior());
    }

}

void handleControlChange (byte channel, byte number, byte value)
{
  // Process our inputs
  robot->handleInput(number, value);

  // Legacy input handling
  Motor::processMotorCC(channel, number, value);
  Sound::processSoundCC(channel, number, value);     
}

// Special-cased event handling
void noteOnControl (byte channel, byte note, byte velocity) {
  NoteControl::noteOnControl(channel, note, velocity);
}

void noteOffControl(byte channel, byte note, byte velocity) {
  NoteControl::noteOffControl(channel, note, velocity);
}





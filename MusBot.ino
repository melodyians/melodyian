#include <EEPROM.h>
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
// #include <Wire.h>

#include "commands.h"
#include "melodyian.h"
#include "midicc.h"
#include "time.h"
#include "state.h" 
#include "inputhandler.h"
#include "smoothing.h"

MIDI_CREATE_DEFAULT_INSTANCE();

Timer * timer;
RobotState * robot_state;
InputHandler * input_handler;
HardwareInterface * hardware_interface;

void setup()  {

  timer = new Timer();

  hardware_interface = new ArduinoInterface();

  initializeState();
  initializeInputs();
  
} 

void initializeState() {

  LEDStorage * led_storage = new LEDStorage();
  led_storage->readFromEEPROM();
  robot_state = new RobotState(led_storage);
}


void initializeInputs() {
  input_handler = new InputHandler(127);

  MIDI.begin();
  Serial.begin(57600);
  MIDI.turnThruOff();
  
  // Think I need to do this so Arduino can work w/ any MIDI message Max will be sending it
  MIDI.setInputChannel(MIDI_CHANNEL_OMNI); 
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandleNoteOn(noteOnControl);

  // Only needed if receiving MIDI Note Off messages from controller device to stop notes
  MIDI.setHandleNoteOff(noteOffControl); 


  input_handler->registerInput(COLORJITTERBYPASS_CC, 
      new StoreMutatedInputValue<bool>(robot_state->bypass_random_color, Smoothing::booleanButton));
  
  input_handler->registerInput(NOTEJITTERBYPASS_CC, 
      new StoreMutatedInputValue<bool>(robot_state->bypass_random_note, Smoothing::booleanButton));

  input_handler->registerInput(JITTER_CC, 
      new StoreMutatedInputValue<float>(robot_state->randomness, Smoothing::mapByteToThousand));

  input_handler->registerInput(RED_CC, 
      new StoreMutatedInputValue<byte>(robot_state->red_slider, Smoothing::smoothRBGFader));

  input_handler->registerInput(GREEN_CC, 
      new StoreMutatedInputValue<byte>(robot_state->green_slider, Smoothing::smoothRBGFader));

  input_handler->registerInput(BLUE_CC, 
      new StoreMutatedInputValue<byte>(robot_state->blue_slider, Smoothing::smoothRBGFader));

  input_handler->registerInput(RATE1_CC, 
      new StoreMutatedInputValue<int>(robot_state->rate, Smoothing::smoothRateFader));

  // TODO: this seems to impact the rate sometimes.
  input_handler->registerInput(FADESPD_CC, 
      new StoreInputValue<unsigned int>(robot_state->decay));

  input_handler->registerInput(SETCOLQ_CC, 
      new StoreMutatedInputValue<bool>(robot_state->color_on, Smoothing::booleanButton));


  input_handler->registerInput(SETCOLQ_CC, 
      new StoreInputValue<unsigned int>(robot_state->current_light_behavior));

  /*
    input_handler->registerInput(TRIGLP1_CC, new LightPresetCommand(robot_state, 1));
    input_handler->registerInput(TRIGLP2_CC, new LightPresetCommand(robot_state, 2));
    input_handler->registerInput(TRIGLP3_CC, new LightPresetCommand(robot_state, 3));
    input_handler->registerInput(TRIGLP4_CC, new LightPresetCommand(robot_state, 4));
    input_handler->registerInput(TRIGLP5_CC, new LightPresetCommand(robot_state, 5));
    input_handler->registerInput(TRIGLP6_CC, new LightPresetCommand(robot_state, 6));
    input_handler->registerInput(TRIGLP7_CC, new LightPresetCommand(robot_state, 7));
    input_handler->registerInput(TRIGLP8_CC, new LightPresetCommand(robot_state, 8));
  */
 

  /*
 
  input_handler->registerInput(DYNAMIC_CC, 
      new StoreMutatedInputValue<bool>(robot_state->new_pulse, Smoothing::booleanButton));
 
  input_handler->registerInput(DYNAMIC_CC, 
      new StoreInputValue<unsigned int>(robot_state->color_pulse));

  input_handler->registerInput(ARMEEPROM_CC, 
      new StoreMutatedInputValue<bool>(robot_state->arm_rec, Smoothing::booleanButton));

  input_handler->registerInput(WRITECOLOR_CC, 
      new StoreMutatedInputValue<bool>(robot_state->save_color, Smoothing::booleanButton));

  */

}




void loop()
{
  // Move our clock forward
  unsigned long dt = timer->step();

  // Battery and Memory housekeeping
  bool midi_read = MIDI.read();

  // Disabled -- this seems to unpredictbly impact 
  // the state of the LED, and I have no idea why.
  //Battery::pingBatVoltage(midi_read, hardware_interface);
  
  // TODO: Pull out to behavior
  // LED::writeEEPROMValues();

  // Update our Robot from our current state
  //Sound::processSoundTriggers(dt, robot_state, hardware_interface);
  LED::updateLEDBehavior(robot_state, hardware_interface, dt);
  //Motor::actuateMotors(hardware_interface);

  debug(dt);
}


unsigned long debug_every = 3000;
unsigned long debug_timer = 0;

void debug(unsigned long dt) {

    debug_timer = debug_timer + dt;
    if (debug_timer > debug_every) {
      debug_timer = 0;
      MidiCC::writeMidiCC(RED_CC, *(robot_state->red_slider));
      MidiCC::writeMidiCC(GREEN_CC, *(robot_state->green_slider));
      MidiCC::writeMidiCC(BLUE_CC, *(robot_state->blue_slider));
    }


}

void handleControlChange (byte channel, byte number, byte value)
{
  // Process our inputs
  input_handler->processInput(channel, number, value);


  // Legacy input handling
  LED::processLEDCC(channel, number, value);
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





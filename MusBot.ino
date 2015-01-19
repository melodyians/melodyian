#include <EEPROM.h>
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
// #include <Wire.h>

#include "melodyian.h"
#include "midicc.h"
#include "time.h"
#include "input.h" 
#include "controlhandler.h"
#include "smoothing.h"

MIDI_CREATE_DEFAULT_INSTANCE();

#define NUM_LIGHT_PRESETS 8

Timer * timer;
InputValues * input_values;
ControlChangeHandler * cc_handler;

void setup()  {

  timer = new Timer();
  input_values = new InputValues(NUM_LIGHT_PRESETS);
  cc_handler = new ControlChangeHandler(127);

  
  MIDI.begin();
  Serial.begin(57600);
  MIDI.turnThruOff();
  
  // Think I need to do this so Arduino can work w/ any MIDI message Max will be sending it
  MIDI.setInputChannel(MIDI_CHANNEL_OMNI); 
  
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandleNoteOn(noteOnControl);

  // Only needed if receiving MIDI Note Off messages from controller device to stop notes
  MIDI.setHandleNoteOff(noteOffControl); 
  
  ArduinoInterface::setupPins();
  
  LED::readFromEEPROM();
  
  ArduinoInterface::stopMotors();

  
  cc_handler->addCommand(COLORJITTERBYPASS_CC, 
      new StoreInputValue<bool>(input_values->bypass_random_color, Smoothing::booleanButton));
  cc_handler->addCommand(NOTEJITTERBYPASS_CC, 
      new StoreInputValue<bool>(input_values->bypass_random_note, Smoothing::booleanButton));
  
} 


void loop()
{
  unsigned long dt = timer->step();

  bool midi_read = MIDI.read();
  Battery::pingBatVoltage(midi_read);

  LED::writeEEPROMValues();
  Sound::processSoundTriggers(dt, input_values);
  LED::processQueue(dt, input_values);
  Motor::actuateMotors();
 
}

void handleControlChange (byte channel, byte number, byte value)
{
  cc_handler->processMessage(channel, number, value);

  LED::processLEDCC(channel, number, value);
  Motor::processMotorCC(channel, number, value);
  Sound::processSoundCC(channel, number, value);     
}

void noteOnControl (byte channel, byte note, byte velocity) {
  NoteControl::noteOnControl(channel, note, velocity);
}

void noteOffControl(byte channel, byte note, byte velocity) {
  NoteControl::noteOffControl(channel, note, velocity);
}





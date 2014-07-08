#include <EEPROM.h>
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
// #include <Wire.h>

#include "melodyian.h"
#include "midicc.h"
#include "time.h"

MIDI_CREATE_DEFAULT_INSTANCE();

Timer * timer;

void setup()  {
  
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

  timer = new Timer();
} 


void loop()
{
  unsigned long dt = timer->step();

  bool midi_read = MIDI.read();
  Battery::pingBatVoltage(midi_read);

  LED::writeEEPROMValues();
  Sound::processSoundTriggers(dt);
  LED::processQueue(dt);
  Motor::actuateMotors();
 
}

void handleControlChange (byte channel, byte number, byte value)
{
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





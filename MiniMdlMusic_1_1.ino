#include <EEPROM.h>
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
#include <Wire.h>


#include "melodyian.h"

#include "midicc.h"
#include "led.h"
#include "colorhelper.h"
#include "easing.h"
#include "motor.h"
#include "melodies.h"
#include "arduinopins.h"
#include "melodies.h"





//=================ARDUINO SETUP FUNCTION=====================
void setup()  {
  
  MIDI.begin();
  Serial.begin(57600);
  MIDI.turnThruOff();
  
  // Think I need to do this so Arduino can work w/ any MIDI message Max will be sending it
  MIDI.setInputChannel(MIDI_CHANNEL_OMNI); 
  
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandleNoteOn(NoteOnControl);

  // Only needed if receiving MIDI Note Off messages from controller device to stop notes
  MIDI.setHandleNoteOff(NoteOffControl); 
  
  ArduinoPins::setupPins();
  
  LED::readFromEEPROM();
  
  ArduinoPins::stopMotors();
} 


void loop()
{
  bool midi_read = MIDI.read();
  Battery::pingBatVoltage(midi_read); 

  LED::writeEEPROMValues();
  Melodies::processMelodyTriggers();
  processMotorMovement();
 
}

void handleControlChange (byte channel, byte number, byte value)
{
  LED::processLEDCC(channel, number, value);
  processMotorCC(channel, number, value);
  Melodies::processMelodyCC(channel, number, value);     
}







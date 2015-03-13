#include <Arduino.h>

#include "actions.h"
#include "melodyian.h"
#include "smoothing.h"
#include "pitches.h"
#include "midicc.h"
#include "state.h"

namespace Sound {

  byte notePosition = 0;

  unsigned long currentMillisTone = 0;
  unsigned long lastNoteOnMillis = 0;

  //int keyStatus[] = {noteC3act, noteG3act, noteA3act};
  //bool keyModeAct = false;   *******remove after enabling below setKeyMode() function********
  //Flags::setKeyMode(false); //********adding for new FLASH queue functionality when keyModeAct() == true**********
  byte melody1NoteLength = 7; //Number of notes in a melody - 1 (the highest number address of matrix addresses in an 8x1 matrix (0-7))


  //general rate value used by altFlash() & playMelody() functions and AUTOFADE light queue pattern 
  int toneRate = 1000; 

  // int randomness = 0; //'randomness' value of melody sequence playback (0 = 100% chance next note in melody sequence is played after the current note, 127 = equal probability of any other note from the sequence being played after the current note)

  int melody1[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0 /*rest*/, NOTE_B3, NOTE_C4};

  unsigned int melody1noteDurations[/*melody1NoteLength + 1*/] = {4, 8, 8, 4,4,4,4,4}; // note durations: 4 = quarter note, 8 = eighth note, etc.

  void playMelody1(unsigned short dt, RobotState * robot_state, HardwareInterface * hardware)
  { 

    int noteJitter = 0;

    if (robot_state->bypassRandomNote() == false) {
      noteJitter = robot_state->randomness();
    }
    //tone(tonePin, melody1[notePosition]); //for debugging
     
    currentMillisTone = millis(); 

    if (notePosition == melody1NoteLength) //if we've reached the last note of the melody
    {
      unsigned int noteDuration = toneRate/melody1noteDurations[notePosition]; // to calculate the note duration, take one second divided by the note type. (e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.)
      unsigned int pauseBetweenNotes = noteDuration * 1.30;  // to distinguish the notes, set a minimum time between them. the note's duration + 30% seems to work well.   
      unsigned int timeElapsed = currentMillisTone - lastNoteOnMillis;
      unsigned int maxNoteEventLength = noteDuration + pauseBetweenNotes;

      byte randomNoteVal = map(noteJitter, 0, 127, 0, melody1NoteLength);
      byte randomNote = 1 + random(0, randomNoteVal);

      if (timeElapsed <= noteDuration)
      {
        Flags::setNoteOn(true);
        hardware->playTone(melody1[notePosition]);       
      }  
      else if (timeElapsed > noteDuration && timeElapsed <= maxNoteEventLength)
      {
        Flags::setNoteOn(false);
        hardware->toneOff(); //silence for pause time 
      }    
      else
      {
        lastNoteOnMillis = currentMillisTone;
        //notePosition = 0;

        notePosition = notePosition + (1 * randomNote);
        if (notePosition > melody1NoteLength)
        {
          notePosition = notePosition - melody1NoteLength - 1; 
        }
        
      }
    }

    else //for all other notes of the melody...
    {
      unsigned int noteDuration = toneRate/melody1noteDurations[notePosition]; // to calculate the note duration, take a time value (2ms - ~10 seconds) divided by the note type. (e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.)
      unsigned int pauseBetweenNotes = noteDuration * 1.30 /*could replace this w/ variable value linked w/ encoder on CC76*/;  // to distinguish the notes, set a minimum time between them. the note's duration + 30% seems to work well.   
      unsigned int timeElapsed = currentMillisTone - lastNoteOnMillis;
      unsigned int maxNoteEventLength = noteDuration + pauseBetweenNotes;

      byte randomNoteVal = map(noteJitter, 0, 127, 0, melody1NoteLength);
      byte randomNote = 1 + random(0, randomNoteVal);


      if (timeElapsed <= noteDuration)
      {
        Flags::setNoteOn(true);
        hardware->playTone(melody1[notePosition]);       
      }  
      
      else if (timeElapsed > noteDuration && timeElapsed <= maxNoteEventLength)
      {
        Flags::setNoteOn(false);
        hardware->toneOff(); //silence for pause time 
      }    
      
      else
      {
        lastNoteOnMillis = currentMillisTone;
        notePosition = notePosition + (1 * randomNote);
        if (notePosition > melody1NoteLength)
        {
          notePosition = notePosition - melody1NoteLength - 1; 
        }
      }
    }
  }

  //-------MELODY #2---------//
  boolean melody2Act = false;
  byte melody2NoteLength = 8; //Number of notes in a melody - 1 (the number of matrix addresses in an 8x1 matrix)

  int melody2[] = {0, NOTE_FS3, NOTE_GS3, NOTE_AS3, NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_F4, NOTE_DS4};

  int melody2noteDurations[/*melody1NoteLength + 1*/] = {8,8,8,8,8,16,16,8,8}; // note durations: 4 = quarter note, 8 = eighth note, etc.




  void playMelody2(unsigned short dt, RobotState * robot_state, HardwareInterface * hardware)
  { 

    int noteJitter = 0;

    if (robot_state->bypassRandomNote() == false) {
      noteJitter = robot_state->randomness();
    }

    currentMillisTone = millis();

    if (notePosition == melody2NoteLength) 
    {
      unsigned int noteDuration = toneRate/melody2noteDurations[notePosition]; // to calculate the note duration, take a time value (2ms - ~10 seconds) divided by the note type. (e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.)
      unsigned int pauseBetweenNotes = noteDuration * 1.30;  // to distinguish the notes, set a minimum time between them. the note's duration + 30% seems to work well.   
      unsigned int timeElapsed = currentMillisTone - lastNoteOnMillis;
      unsigned int maxNoteEventLength = noteDuration + pauseBetweenNotes;
      
      byte randomNoteVal = map(noteJitter, 0, 127, 0, melody2NoteLength);
      byte randomNote = 1 + random(0, randomNoteVal);
      
      if (timeElapsed <= noteDuration)
      {
        Flags::setNoteOn(true);
        hardware->playTone(melody2[notePosition]);       
      }  
      else if (timeElapsed > noteDuration && timeElapsed <= maxNoteEventLength)
      {
        Flags::setNoteOn(false);
        hardware->toneOff();       

      }    
      else
      {
        lastNoteOnMillis = currentMillisTone;
        
        ///
        notePosition = notePosition + (1 * randomNote);
        if (notePosition > melody2NoteLength)
        {
          notePosition = notePosition - melody2NoteLength - 1; 
        }
        ///
        
      }
    }

    else
    {
      unsigned int noteDuration = toneRate/melody2noteDurations[notePosition]; // to calculate the note duration, take a time value (2ms - ~10 seconds) divided by the note type. (e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.)
      unsigned int pauseBetweenNotes = noteDuration * 1.30 /*could replace this w/ variable value linked w/ encoder on CC76*/;  // to distinguish the notes, set a minimum time between them. the note's duration + 30% seems to work well.   
      unsigned int timeElapsed = currentMillisTone - lastNoteOnMillis;
      unsigned int maxNoteEventLength = noteDuration + pauseBetweenNotes; //time of note duration and following rest (space between notes)
      
      ///
      byte randomNoteVal = map(noteJitter, 0, 127, 0, melody2NoteLength); //scale MIDI CC value range to the amount of addresses of the the melody2 matrix.
      byte randomNote = 1 + random(0, randomNoteVal); //pick a random note number (add 1 to always be able to advance melody even when noteJitter = 0.)
      ///
      
      if (timeElapsed <= noteDuration)
      {
        Flags::setNoteOn(true);
        hardware->playTone(melody2[notePosition]);       
      }  
      
      else if (timeElapsed > noteDuration && timeElapsed <= maxNoteEventLength)
      {
        Flags::setNoteOn(false);
        hardware->toneOff(); //silence for pause time 
      }    
      
      else
      {
        lastNoteOnMillis = currentMillisTone;
        
        notePosition = notePosition + (1 * randomNote);

        if (notePosition > melody2NoteLength) //if notePosition is greater than the total melody note count, convert it to an equivelant position value within the melody note count
        {
          notePosition = notePosition - melody2NoteLength - 1; //need to subtract 1 to be able to return to first note of melody if needed (result of having added 1 to randomNote value above)
        } 
      }
    }  
  }


  void turnOffPowerIfOn(HardwareInterface * hardware) {
    if (hardware->ampIsOn()) {
      hardware->setAmpPower(false);
    } 
  }

  void turnOnPowerIfOff(HardwareInterface * hardware) {
    if (!hardware->ampIsOn()) {
      hardware->setAmpPower(true);
    }
  }

  // TODO: Pass in Note Control stuff
  void processSoundTriggers(unsigned long dt, RobotState * robot_state, HardwareInterface * hardware) {

    if (Flags::melodyOneAct() == false && Flags::melodyTwoAct() == false && /*keyModeAct == false*/Flags::keyModeAct() == false) //<-----**********uncomment for new FLASH when keyModeAct FUNCTIONALITY *********
    {
      hardware->toneOff();
      turnOffPowerIfOn(hardware);
    }

    else if (Flags::melodyOneAct() )
    {
      turnOnPowerIfOff(hardware);
      Flags::setMelodyTwo(false); //I think this is needed to prevent melody performance conflicts if both melody trigger buttons are toggled 'on' at the same time...but will 1 loop of lag be audible?
      //keyModeAct = false;  //remove if below line enabled
      Flags::setKeyMode(false);  //********Add for new FLASH - keyModeAct flag Functionality******
      playMelody1(dt, robot_state, hardware);
    }
    else if (Flags::melodyTwoAct() )
    {
      turnOnPowerIfOff(hardware);
      Flags::setMelodyOne(false);
      //keyModeAct = false;  //remove if below line enabled
      Flags::setKeyMode(false);  //********Add for new FLASH - keyModeAct flag Functionality******
      playMelody2(dt, robot_state, hardware);
    }
    else  //MANUAL MIDI NOTE PERFORMANCE
    {
      turnOnPowerIfOff(hardware);
      Flags::setMelodyOne(false);
      Flags::setMelodyTwo(false);
      if (NoteControl::anyActingNotes()){
        Flags::setNoteOn(true);  //********Add for new FLASH - keyModeAct flag Functionality******
        hardware->playTone(NoteControl::currentHz());
      }
      else {
        Flags::setNoteOn(false);  //********Add for new FLASH - keyModeAct flag Functionality******
        hardware->toneOff();
      }

      if (Flags::MIDInotePanic() ) {
          hardware->toneOff();
          NoteControl::allNoteOffControl();
          Flags::setNoteOn(false);
          Flags::setMIDInotePanic(false);
      }

    }

  }


  void processSoundCC(byte channel, byte number, byte value) {
     //========MUSIC RELATED

    if(number == RATE1_CC) //RATE #1  (knob pot 'B5' on axiom)
    {
      toneRate = Smoothing::smoothRateFader(value);
    }

    /*
    if (number == JITTER_CC) //COLOR RANDOMNESS / JITTER CONTROL (knob B8)
    {
      randomness = value;
    }
    */

    if (number == MEL1TRIG_CC)
    {
      if (value == 127) {
        Flags::setMelodyOne(true);
      } else {
        Flags::setMelodyOne(false);
        notePosition = 0;
      } 
    }
    
    if(number == MEL2TRIG_CC)
    {
      if (value == 127) {
        Flags::setMelodyTwo(true);
      } else {
        Flags::setMelodyTwo(false);
        notePosition = 0;
      } 
    }

    if (number == KEYACT_CC)
    {
      if (value == 127) {
        //keyModeAct = true;  // remove if below line is enabled
        Flags::setKeyMode(true); //********Add if new FLASH - keyModeAct flag Functionality******
      } else {
        //keyModeAct = false;  // remove if below line is enabled
        Flags::setKeyMode(false); //********Add if new FLASH - keyModeAct flag Functionality******
      }
    }

    if (number == MIDIPANIC_CC)
    {
      if (value == 127) {Flags::setMIDInotePanic(true);}
      else {Flags::setMIDInotePanic(false);}
    }
  }
}

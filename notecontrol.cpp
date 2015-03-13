#include <Arduino.h>
#include "melodyian.h"
//#include "pitches.h"


// TODO: Turn into SoundState
namespace NoteControl {
  
  const unsigned int NUM_NOTES = 112;

  // Middle C (C4)
  int last_note_on = 60; 

  // Special C syntax to initialize entire array to 'false'
  bool acting_notes[NUM_NOTES] = { false };

  // http://www.phys.unsw.edu.au/jw/notes.html
  unsigned int frequencyFromMidiNote (float note) {
    return (unsigned int) (pow(2, (note - 69.0) / 12.0) * 440.0);
  };

  bool anyActingNotes() {
    for (int i = 0; i <= NUM_NOTES; i++) {
      if (acting_notes[i]) {
        return true;
      }
    }
    return false;
  }

  unsigned int currentHz() {
    return frequencyFromMidiNote(last_note_on);
  }


  void noteOnControl (byte channel, byte note, byte velocity) {
    if (velocity > 0) {
      acting_notes[note] = true;
      last_note_on = note;
    } else {
      acting_notes[note] = false;
    }
  }

  // Only needed if receiving MIDI Note Off messages from controller device to stop notes
  // (TouchOSC does this, but Lemur and other controllers commonly send Note On messages 
  //  w/ a 0 velocity value to turn notes off)
  void noteOffControl(byte channel, byte note, byte velocity) {
    acting_notes[note] = false;
  }

  //**********adding for new MIDInotePanic control functionality...
  void allNoteOffControl() { 
    for (int i = 0; i <= NUM_NOTES; i++)
      {acting_notes[i] = false;}
  }

}

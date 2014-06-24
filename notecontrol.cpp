#include "melodyian.h"
#include "pitches.h"

namespace NoteControl {
  
  bool noteC3act = false;
  bool noteCS3act = false;
  bool noteD3act = false;
  bool noteDS3act = false;
  bool noteE3act = false;
  bool noteF3act = false;
  bool noteFS3act = false;
  bool noteG3act = false;
  bool noteGS3act = false;
  bool noteA3act = false;
  bool noteAS3act = false;
  bool noteB3act = false;
  bool noteC4act = false;


  int lastNoteOn = 60; //middle C (C4)
  //byte lastNoteOff = 60;

  bool anyActingNotes() {
    return 
          noteC3act  ||
          noteCS3act ||
          noteD3act  ||
          noteDS3act ||
          noteE3act  ||
          noteF3act  ||
          noteFS3act ||
          noteG3act  ||
          noteGS3act ||
          noteA3act  ||
          noteAS3act ||
          noteB3act  ||
          noteC4act;
  }

  int lastNote() {
    return lastNoteOn;
  }


  //==============MIDI NOTE CALL BACK================
/*
  void HandleNoteOn(byte channel, byte note, byte velocity);
*/

  void noteOnControl (byte channel, byte note, byte velocity) 
  {
    if (note == MIDINOTE_C3)
    {
      if (velocity > 0)
      {
        noteC3act = true;
        lastNoteOn = NOTE_C3;
      }

      else {noteC3act = false;}
    }

    if (note == MIDINOTE_CS3)
    {
      if (velocity > 0)
      {
        noteCS3act = true;
        lastNoteOn = NOTE_CS3;
      }

      else {noteCS3act = false;}
    }

    if (note == MIDINOTE_D3)
    {
      if (velocity > 0)
      {
        noteD3act = true;
        lastNoteOn = NOTE_D3;
      }

      else {noteD3act = false;}
    }

    if (note == MIDINOTE_DS3)
    {
      if (velocity > 0)
      {
        noteDS3act = true;
        lastNoteOn = NOTE_DS3;
      }

      else {noteDS3act = false;}
    }

    if (note == MIDINOTE_E3)
    {
      if (velocity > 0)
      {
        noteE3act = true;
        lastNoteOn = NOTE_E3;
      }

      else {noteE3act = false;}
    }

    if (note == MIDINOTE_F3)
    {
      if (velocity > 0)
      {
        noteF3act = true;
        lastNoteOn = NOTE_F3;
      }

      else {noteF3act = false;}
    }

    if (note == MIDINOTE_FS3)
    {
      if (velocity > 0)
      {
        noteFS3act = true;
        lastNoteOn = NOTE_FS3;
      }

      else {noteFS3act = false;}
    }

    if (note == MIDINOTE_G3)
    {
      if (velocity > 0)
      {
        noteG3act = true;
        lastNoteOn = NOTE_G3;
      }

      else {noteG3act = false;}
    }

    if (note == MIDINOTE_GS3)
    {
      if (velocity > 0)
      {
        noteGS3act = true;
        lastNoteOn = NOTE_GS3;
      }

      else {noteGS3act = false;}
    }

    if (note == MIDINOTE_A3)
    {
      if (velocity > 0)
      {
        noteA3act = true;
        lastNoteOn = NOTE_A3;
      }

      else {noteA3act = false;}
    }

    if (note == MIDINOTE_AS3)
    {
      if (velocity > 0)
      {
        noteAS3act = true;
        lastNoteOn = NOTE_AS3;
      }

      else {noteAS3act = false;}
    }

    if (note == MIDINOTE_B3)
    {
      if (velocity > 0)
      {
        noteB3act = true;
        lastNoteOn = NOTE_B3;
      }

      else {noteB3act = false;}
    }

    if (note == MIDINOTE_C4)
    {
      if (velocity > 0)
      {
        noteC4act = true;
        lastNoteOn = NOTE_C4;
      }

      else {noteC4act = false;}
    }

  }


/*
  void HandleNoteOff(byte channel, byte note, byte velocity);  //only needed if receiving MIDI Note Off messages from controller device to stop notes
                                                               //(TouchOSC does this, but Lemur and other controllers commonly send Note On messages w/ a 0 velocity value to turn notes off)
*/
  void noteOffControl(byte channel, byte note, byte velocity)  //only needed if receiving MIDI Note Off messages from controller device to stop notes
  {
    if (note == MIDINOTE_C3) {noteC3act = false;}
    if (note == MIDINOTE_CS3) {noteCS3act = false;}
    if (note == MIDINOTE_D3) {noteD3act = false;}
    if (note == MIDINOTE_DS3) {noteDS3act = false;}
    if (note == MIDINOTE_E3) {noteE3act = false;}
    if (note == MIDINOTE_F3) {noteF3act = false;}
    if (note == MIDINOTE_FS3) {noteFS3act = false;}
    if (note == MIDINOTE_G3) {noteG3act = false;}
    if (note == MIDINOTE_GS3) {noteGS3act = false;}
    if (note == MIDINOTE_A3) {noteA3act = false;}
    if (note == MIDINOTE_AS3) {noteAS3act = false;}
    if (note == MIDINOTE_B3) {noteB3act = false;}
    if (note == MIDINOTE_C4) {noteC4act = false;}
  }

}

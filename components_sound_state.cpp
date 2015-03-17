#include "components.h"


unsigned int FrequencyFromMidiNote (float note) {
    return (unsigned int) (pow(2, (note - 69.0) / 12.0) * 440.0);
};


SoundState::SoundState(unsigned int num_notes) {

    this->num_notes = num_notes;
    this->last_note_on = 60; // default value
    this->acting_notes = new bool[this->num_notes];

    for (int i = 0; i < this->num_notes; i++) {
        this->acting_notes[i] = false;
    }

}

bool SoundState::anyActingNotes() {
    for (int i = 0; i <= this->num_notes; i++) {
      if (this->acting_notes[i]) {
        return true;
      }
    }
    return false;

}

unsigned int SoundState::currentHz() {
    return FrequencyFromMidiNote(this->last_note_on);
}

void SoundState::turnOffAllNotes() {
    for (int i = 0; i <= this->num_notes; i++) {
      this->acting_notes[i] = false;
    }
}

void SoundState::turnNoteOn(int note) {
    this->acting_notes[note] = true;
    this->last_note_on = note;
}

void SoundState::turnNoteOff(int note) {
    this->acting_notes[note] = false;
}
  

bool SoundState::isNoteOn(int note) {
    return this->acting_notes[note];
}

#include "components.h"
#include "types.h"


byte MaxRandomNoteValue(float random_jitter, int total_num_notes) {
    return (byte) (random_jitter * total_num_notes);
  }

Melody::Melody(byte * melody, byte * durations) {
    this->melody = melody;
    this->durations = durations;
    this->length = sizeof(melody) / sizeof(byte);

    this->note_position = 0;
    this->elapsed = 0;
}

void Melody::reset() {
    this->note_position = 0;
    this->elapsed = 0;    
}

unsigned int Melody::current_note_duration(int rate) {
    // To calculate the note duration, take a time value (2ms - ~10 seconds) 
    // divided by the note type. (e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.)
    return rate / this->durations[this->note_position];
}


byte Melody::current_note() {
    return this->melody[this->note_position];
}

void Melody::play(unsigned short dt, State * robot_state) {

    float jitter = 0;

    if (robot_state->bypassRandomNote() == false) {
      jitter = robot_state->randomness();
    }

    this->elapsed += dt;

    unsigned int current_note_duration = this->current_note_duration(robot_state->rate());
    unsigned int pause_duration = current_note_duration * 1.30; 
    unsigned int max_note_event_length = current_note_duration + pause_duration;

    // If we are still playing the note
    if (this->elapsed <= current_note_duration) {
        robot_state->sound_state()->turnNoteOn(this->current_note());
   
    // If we are during the pause after the note
    } else if (this->elapsed <= max_note_event_length) {
        robot_state->sound_state()->turnNoteOff(this->current_note());

    // Progress to next note next time around
    } else {
        this->elapsed = 0;

        // Add jitter
        byte max_offset = MaxRandomNoteValue(jitter, this->length);
        byte random_note = 1 + random(0, max_offset);
        
        this->note_position = this->note_position  + (1 * random_note);
        if (this->note_position >= this->length) {
          this->note_position = this->note_position - this->length - 1; 
        }

    }
}
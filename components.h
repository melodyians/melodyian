#pragma once

#include "types.h"

class HardwareInterface {

public:
  virtual void writeToLED(byte red, byte green, byte blue) = 0;

  virtual void moveMotor(int motor, int motorSpeed, int motorDirection) = 0;
  virtual void stopMotors() = 0;

  virtual int readBattery() = 0;

  virtual void playTone(unsigned int freq) = 0;
  virtual void toneOff() = 0;
  virtual bool ampIsOn() = 0;
  virtual void setAmpPower(bool on) = 0;

};

class InputValues {

private:
    byte offset;
    byte * input_values;
    byte getCCIndex(byte control_number);

public:
    InputValues(byte lowest_cc);
    void storeInput(byte control_number, byte value);
    byte getValue(byte control_number);

};

class Output {

public:
  Output();
  byte r;
  byte g;
  byte b;

  bool amp_on;
  unsigned int tone;

  void setColor(byte r, byte g, byte b);
  void setColorBlack();

  void setAmpPower(bool toggle);

};

class LEDStorage {

private:
    int * lightPresetData;
    int ** lightPresetArray;
    int* getLightPresetPtr(int i);

public:

    LEDStorage();

    RGBColor getLightPresetColor(int i);
    void setPresetColor(int i, byte r, byte g, byte b);
    
    void readFromEEPROM();
    void saveToEEPROM(int lightPresetVal);
};


// Output and State

class SoundState {

private:
  int num_notes;
  int last_note_on;
  bool * acting_notes;

public: 
  SoundState(unsigned int num_notes);
  bool anyActingNotes();
  unsigned int currentHz();

  void turnNoteOn(int note);
  void turnNoteOff(int note);
  void turnOffAllNotes();

  bool isNoteOn(int note);

};

class State {

private:
    InputValues * input_values;
    LEDStorage * led_storage_; // TODO probably best private
    SoundState * sound_state_;

public:
    State();

    LEDStorage * led_storage();
    SoundState * sound_state();

    void updateInput(byte control_number, byte value);

    // Shared Buttons
    bool bypassRandomColor();
    bool bypassRandomNote();

    float randomness();
    unsigned int decay();
    int rate();
    
    // LED
    byte ledRedValue();
    byte ledGreenValue();
    byte ledBlueValue();

    byte pulseValue();

    bool colorOn();

    bool recordEEPROMArmed();
    bool saveColorOn();
    void disarmRecordEEPROM();

    void setCurrentLEDValues(byte r, byte g, byte b);

    // Sound
    bool soundActionOn();
    bool melodyOneOn();
    bool melodyTwoOn();
    bool keyModeOn(); 
    bool midiPanicOn();

    // TODO -- Motor

};


class Behavior {

protected:
    unsigned short timer;
    byte behavior_key;
    void setCurrentBehavior(byte behavior_key);
    void clearCurrentBehavior();
    void resetTimer();
    void incrementTimer(unsigned short dt);
    void decrementTimer(unsigned short dt);

public:
    Behavior();
    virtual void updateBehavior(unsigned short dt, State * state, Output * output) = 0;
    virtual void updateBehaviorKey(byte control_number, byte value) = 0;
    byte getCurrentBehavior();

};

class BatteryReader {

private:
  HardwareInterface * hardware;
  unsigned short timer;
  byte analogVoltageToMidiValue(int analog_value);

public:
    BatteryReader(HardwareInterface * hardware);
    void readBattery(unsigned short dt, bool midi_read);
};



class Melody {

private:
  byte length;
  byte * melody;
  byte * durations;
  byte note_position;
  int elapsed;
  unsigned int current_note_duration(int rate);

public:
  Melody(byte * melody, byte * durations);
  void reset();
  void play(unsigned short dt, State * robot_state);
  byte current_note();
};
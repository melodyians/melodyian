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

  void setColor(byte r, byte g, byte b);
  void setColorBlack();

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

class State {

private:
    InputValues * input_values;

public:
    State();
    LEDStorage * led_storage; // TODO probably best private

    void updateInput(byte control_number, byte value);

    bool bypassRandomColor();
    bool bypassRandomNote();

    float randomness();
    unsigned int decay();
    int rate();
    
    byte ledRedValue();
    byte ledGreenValue();
    byte ledBlueValue();

    byte pulseValue();

    bool colorOn();

    bool recordEEPROMArmed();
    bool saveColorOn();
    void disarmRecordEEPROM();

    void setCurrentLEDValues(byte r, byte g, byte b);

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
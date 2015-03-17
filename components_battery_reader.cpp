#include "components.h"

#include "constants_actions.h"
#include "helper_midi.h"


BatteryReader::BatteryReader(HardwareInterface * hardware) {
    this->hardware = hardware;
    this->timer = 0;
}

byte BatteryReader::analogVoltageToMidiValue(int analog_value) {
    // Simplify by just mapping our analog value directly to a midi value
    return map(analog_value, 0, 1023, 0, 127);
}

void BatteryReader::readBattery(unsigned short dt, bool midi_read) {
    this->timer += dt;

    // After 5 seconds, ping the battery and output value
    // if we didn't read a midi message this loop.
    if (this->timer >= 5000 && midi_read == false) {

        // Read the voltage on battery 1 divider
        int analog_balue = hardware->readBattery();  
        byte midi_value = this->analogVoltageToMidiValue(analog_balue);
        MidiOut::WriteMidiCC(BAT1VOLTREAD_CC, midi_value);

        this->timer = 0;
    }
}
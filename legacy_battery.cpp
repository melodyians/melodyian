#include "types.h"
#include "legacy.h"

#include "constants_actions.h"
#include "helper_midi.h"

namespace Battery {

    unsigned int bat1MonPinVal = 0;  // variable for the A/D value
    //int bat2MonPinVal = 0;  // variable for the A/D value

    float bat1MonPinVoltage = 0; // variable to hold the calculated voltage
    //float bat2MonPinVoltage = 0; // variable to hold the calculated voltage

    float bat1Voltage = 0;
    //float bat2Voltage = 0;

    float batRatio = 1.64;  //<---new adjusted ratio | Original MEASURED ratio of battery 1 circuit (battery1 voltage / divider1 out voltage) (4.06/2.95) = 1.376

    unsigned long currentMillis = 0;
    unsigned long previousMillis = 0;

    // This seems like it might be causing issues with dropped messages, 
    // not sure if Arduino is able to send and receive MIDI messages at same time
    // comment this out to ensure smoother operation for now...
    void pingBatVoltage(bool midi_read, HardwareInterface * hardware) {

      currentMillis = millis();
      unsigned int timeElapsed = currentMillis - previousMillis;

      if (timeElapsed >= 5000 && midi_read == false) {
        //Read voltage at analog pins....
        bat1MonPinVal = hardware->readBattery();  // read the voltage on battery 1 divider
        //bat2MonPinVal = analogRead(bat2MonPin);  // read the voltage on battery 2 divider  
        
        //Calculate and convert voltage to measure actual battery voltage...
        bat1MonPinVoltage = bat1MonPinVal * 0.0041;   //  Calculate the voltage on the A/D pin
                                                     //  A reading of 1 for the A/D = 0.0041 (Battery voltage / 1024 analog pin resolution) (4.2/1024)
                                                     //  if we multiply the A/D reading by 0.0041 then we get the voltage on the pin.
                      
        //bat2MonPinVoltage = bat2MonPinVal * 0.0041;                               
                                                                         
        bat1Voltage = bat1MonPinVoltage * batRatio;    // Multiply sensed voltage from analog monitor pin times (battery voltage : voltage divider voltage) ratio
        //bat2Voltage = bat2MonPinVoltage * batRatio;    // plus additional factoring to compute actual battery voltage.
      
        /*
        if (bat1Voltage <= 3.3) {bat1Voltage = 3.3;}
        if (bat1Voltage >= 4.2) {bat1Voltage = 4.2;}
        */

        bat1Voltage = constrain(bat1Voltage, 3.3, 4.2);
        //if (bat2Voltage <= 3.3) {bat2Voltage = 3.3;}
        
        unsigned int bat1VoltageIntConv = bat1Voltage * 100;
        //int bat2VoltageIntConv = bat2Voltage * 100;
        
        //Scale readings...
        byte bat1VoltageCCVal = map(bat1VoltageIntConv, 330, 420, 0, 127);
        //byte bat2VoltageCCVal = map(bat2VoltageIntConv, 330, 420, 0, 127);
        
        // TODO re-enable    
        MidiCC::WriteMidiOut(BAT1VOLTREAD_CC, bat1VoltageCCVal);

        //MIDICC_TX(31, bat2VoltageCCVal);
           
        /*
        //Transmit voltage readings as CC messages...
        MIDI.sendControlChange(30, bat1VoltageCCVal, 1);
        MIDI.sendControlChange(31, bat2VoltageCCVal, 1);
        */
        
        /*
        Serial.print("bat1VoltageCCVal = ");
        Serial.print(bat1VoltageCCVal);
        Serial.print(" | bat2VoltageCCVal = ");
        Serial.println(bat2VoltageCCVal);
        */
        previousMillis = currentMillis;
      }
    }
}




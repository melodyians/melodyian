

//====================================MM MINI MODEL CODE============================
#include <EEPROM.h>
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
#include <Wire.h>


#define RED_CC 20  //CC controlling 'Red' amount of LED (0 = red LED off, 127 = red LED at 100% brightness)
#define GREEN_CC 21 //CC controlling 'Green' amount of LED
#define BLUE_CC 22 //CC controlling 'Blue' amount of LED
#define DYNAMIC_CC 23 //Not actively used right now...CC value controlling amount of LED brightness for currently unused light queue pattern. Ask Scott if you're curious about this.

#define MSTRMTRSPD_CC 27 //CC controlling 'master' motor speed (0 = motors off, 127 = full speed) both motors determine their 'actual' speed based off this value
#define MTRDIRREV_CC 58 //CC allowing toggle of motor direction (0 = standard 'forward' direction based off steering control, 127 = reversal of all current motor directions)
#define MTRACT_CC 59 //CC toggling motor activation (0 = motors off, 127 = motors on)
#define MTRSTEER_CC 71 //CC controlling 'steering' direction of robots two motors (0 = complete counter-clockwise rotation, 64 = straight forward direction, 127 = complete clockwise rotation, and everything in between is a linear scaling of these....)

#define ARMEEPROM_CC 60  //CC activating arm EEPROM write mode for saving LED color presets (0 = unarmed, 127 = armed)
#define WRITECOLOR_CC 90 //CC executes write to EEPROM of LED color preset values (127 = write, 0 = don't write)

#define MEL1TRIG_CC 28 //CC triggers melody1 loop (0 = off, 127 = active)
#define MEL2TRIG_CC 29 //CC triggers melody2 loop (0 = off, 127 = active)
#define KEYACT_CC 35 //CC activates manual 'keyboard' style note control (0 = off, 127 = active)

#define BAT1VOLTREAD_CC 30 //CC number whose value (0-127) represents a scaled reading of the robots battery level

#define SETCOLQ_CC 52 //CC activates basic 'Set Color' light queue (static LED color control) (0 = inactive, 127 = active)
#define FLASHQ_CC 53 //CC activates the 'Flash' light queue (0 = inactive, 127 = active)
#define AUTOFADEQ_CC 54 //CC activates the 'Auto Fade' light queue (0 = inactive, 127 = active)
#define DYNAMICQ_CC 55  //CC activates the 'Dynamic Pulse' light queue (not currently an active feature of the robot) (0 = inactive, 127 = active)

#define RATE1_CC 75  //CC whose value (0-127) is scaled to control the frequency of various functions (LED flash rate, melody tempo, autofade rate)
#define FADESPD_CC 77 //CC whose value (0-127) is scaled to control the decay speed of the LEDs brightness when using the 'Dynamic Pulse' queue functionality (currently not in use)
#define JITTER_CC 78 //CC whose value (0-127) is scaled to control the amount of colorJitter and noteJitter (randomness amount)

#define TRIGLP1_CC 80 //CC activating the recall or saving to Light Preset 1 (0 = uncalled, 127 = recall LED RGB values of lightPreset1 or save to lightPreset1)
#define TRIGLP2_CC 81 //CC activating the recall or saving to Light Preset 2 (0 = uncalled, 127 = recall LED RGB values of lightPreset2 or save to lightPreset2)
#define TRIGLP3_CC 82 //CC activating recall or writing to Light Preset 3
#define TRIGLP4_CC 83 //CC activating recall or writing to Light Preset 4
#define TRIGLP5_CC 84 //CC activating recall or writing to Light Preset 5
#define TRIGLP6_CC 85 //CC activating recall or writing to Light Preset 6
#define TRIGLP7_CC 86 //CC activating recall or writing to Light Preset 7
#define TRIGLP8_CC 87 //CC activating recall or writing to Light Preset 8

//==========VARIABLE DECLARATIONS FOR EVERYTHING

byte fdr1; //currently used to store 'Red' value of LED
byte fdr2; //currently used to store 'Green' value of LED
byte fdr3; //currently used to store 'Blue' value of LED
byte fdr4; //currently used to store CC value of 'DYNAMICQCC' (a light queue which will pulse the LED when receiving an external CC message derived from live audio. Not actively used right now, but will in the future. Ask Scott if you're curious for more details about this.

int modWheel = 1; //**********THIS IS NOT BEING USED FOR ANYTHING RIGHT NOW*******//
int queue; //the value of this variable determines which "light queues/patterns" are triggered in the loop() function below

boolean setColorAct = false;

int rate1 = 1000; //general rate value used by altFlash() & playMelody() functions and AUTOFADE light queue pattern 
int lightPresetSelect = 1; //variable for storing last selected lightPreset value (1-8)
int fadeSpeed = 0;  //will be used by 'DYNAMICQCC' light pattern, but not actively used now. Ask Scott if you're curious for more details....
float colorJitter = 0;  //variable used to randomize LED color

boolean lightOnState = false;

boolean colorStoreReached = true;  //*****ONLY BEING USED IN INACTIVE fadeBetween() FUNCTION ******//
int colorStore[3]; //*****ONLY BEING USED IN INACTIVE fadeBetween() FUNCTION ******//

int lightPresetVal;
int lightPresetData[24];
int* lightPreset1 = lightPresetData; //0 1 2 lightPreset1[0] == lightPresetData[0], lightPreset1[3] == lightPreset2[0]
int* lightPreset2 = lightPresetData + 3*1; //3 4 5
int* lightPreset3 = lightPresetData + 3*2;
int* lightPreset4 = lightPresetData + 3*3;
int* lightPreset5 = lightPresetData + 3*4;
int* lightPreset6 = lightPresetData + 3*5;
int* lightPreset7 = lightPresetData + 3*6;
int* lightPreset8 = lightPresetData + 3*7;

int activeLightPreset;
int* getLightPresetPtr(int i)
{
  return lightPreset1 + (i-1)*3;
}

int* getActiveLightPreset()
{
   if(activeLightPreset > 8)
      activeLightPreset = 8;
  
   return getLightPresetPtr(activeLightPreset); 
}

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

//----BATTERY VOLTAGE MONITOR VARIABLES
unsigned long previousBatPingMillis = 0;

byte bat1MonPin = 0;    // input pin for battery 1 divider
//byte bat2MonPin = 1;  //input pin for battery 2 divider

unsigned int bat1MonPinVal = 0;  // variable for the A/D value
//int bat2MonPinVal = 0;  // variable for the A/D value

float bat1MonPinVoltage = 0; // variable to hold the calculated voltage
//float bat2MonPinVoltage = 0; // variable to hold the calculated voltage

float bat1Voltage = 0;
//float bat2Voltage = 0;

float batRatio = 1.64;  //<---new adjusted ratio | Original MEASURED ratio of battery 1 circuit (battery1 voltage / divider1 out voltage) (4.06/2.95) = 1.376


//================LED COLOR PRESET STORAGE & RECALL FUNCTIONS================
boolean armEEPROMwrite = false;
boolean pShift = false; //would like to rename, will eventually be a virtual button to toggle the arming of EEPROMwrite...

void saveToEEPROM(int lightPresetVal)
{
    int a = 0;
    int adrs = (lightPresetVal - 1) * 3;
    for (int i = adrs; i <= (adrs+2); i++)
    {
      EEPROM.write(i, getLightPresetPtr(lightPresetVal)[a]);
      a++;    
    } 
}

void readFromEEPROM()
{  
  for (int i=0; i<=23; i++)
  { 
    lightPresetData[i] = EEPROM.read(i);
  }
}

//============LED PIXEL COLOR CONVERSION AND BRIGHTNESS FUNCTIONS==========
int millisPerCycle;
int offset;

float brightness = 1;
uint32_t LEDcolor;

#define MIN_FADE_TIME_TIMES_ONE_HUNDRED 10
#define MAX_FADE_TIME_TIMES_ONE_HUNDRED 150

//Convert a given HSV (Hue Saturation Value) to RGB(Red Green Blue) and set the led to the color
//  h is hue value, integer between 0 and 360
//  s is saturation value, double between 0 and 1
//  v is value, double between 0 and 1
uint32_t HSVtoRGB(int h, double s, double v) {
  //this is the algorithm to convert from RGB to HSV
  double r=0; 
  double g=0; 
  double b=0;

  double hf=h/60.0;

  int i=(int)floor(h/60.0);
  double f = h/60.0 - i;
  double pv = v * (1 - s);
  double qv = v * (1 - s*f);
  double tv = v * (1 - s * (1 - f));

  switch (i)
  {
  case 0: //rojo dominante
    r = v;
    g = tv;
    b = pv;
    break;
  case 1: //verde
    r = qv;
    g = v;
    b = pv;
    break;
  case 2: 
    r = pv;
    g = v;
    b = tv;
    break;
  case 3: //azul
    r = pv;
    g = qv;
    b = v;
    break;
  case 4:
    r = tv;
    g = pv;
    b = v;
    break;
  case 5: //rojo
    r = v;
    g = pv;
    b = qv;
    break;
  }

  //set each component to a integer value between 0 and 255
  byte red = (byte)constrain((int)255*r,0,255);
  byte green = (byte)constrain((int)255*g,0,255);
  byte blue = (byte)constrain((int)255*b,0,255);
  return Color(red,green,blue);
}

void decay(int elapsed_millis) {
  float delta_t = (elapsed_millis / 1000.0f) / (map(fadeSpeed,0,127,MIN_FADE_TIME_TIMES_ONE_HUNDRED,MAX_FADE_TIME_TIMES_ONE_HUNDRED) / 100.0f);
  brightness = brightness - delta_t;
}

int r(uint32_t color) //I think this return the 'R' value from a 3 byte color value (uint32_t)
{
  return (color & 0xFF0000) >> 16;
}

int g(uint32_t color) //I think this return the 'G' value from a 3 byte color value (uint32_t)
{
  return (color & 0xFF00) >> 8;
}

int b(uint32_t color) //I think this return the 'B' value from a 3 byte color value (uint32_t)
{
  return (color & 0xFF);
}

float xr;
float xg;
float xb;

//===========RGB LED SETUP===========
int redPin = 9; //LED 'Red' connected to pin 9
int greenPin = 10; //LED 'Green' connected to pin 10
int bluePin = 11; //LED 'Blue' connected to pin 11

int transColor[3] = {0, 0, 0}; //starts off as 'black' (no light)


//=========MOTOR DRIVER SETUP============
//motor A connected between A01 and A02 on motor driver breakout
//motor B connected between B01 and B02 on motor driver breakout

byte STBY = 4; //standby pin

//Motor A
byte PWMA = 6; //Speed control of motor A
byte AIN1 = 7; //Direction pin
byte AIN2 = 8; //Direction pin

//Motor B
byte PWMB = 5; //Speed control of motor B
byte BIN1 = 3; //Direction - moved to 3 from 6 
byte BIN2 = 2; //Direction pin

int motorSpdVal = 0; //Master motor speed value
int steerDirection = 64; //steering direction value (0 = complete counter-clockwise rotation around center (both motors spinning in opposite directions at same speed), 64 = straight forwards (both motors spinning in same direction at same speed), 127 = complete clockwise rotation around center )

boolean motorAon = false;
boolean motorBon = false;

int motorAspeed = 0;
int motorBspeed = 0;
boolean motorAdirection = 0;
boolean motorBdirection = 0;

/////////DEBUG///////////
//unsigned int debugCntr = 0;

//===========TONE SETUP==========

#include "pitches.h"

byte tonePin = 12;
byte ampSDNpin = 13;
byte notePosition = 0;

unsigned long currentMillisTone = 0;
unsigned long lastNoteOnMillis = 0;

boolean noteIsOn = false;

boolean keyModeAct = false;

boolean noteC3act = false;
boolean noteCS3act = false;
boolean noteD3act = false;
boolean noteDS3act = false;
boolean noteE3act = false;
boolean noteF3act = false;
boolean noteFS3act = false;
boolean noteG3act = false;
boolean noteGS3act = false;
boolean noteA3act = false;
boolean noteAS3act = false;
boolean noteB3act = false;
boolean noteC4act = false;

int lastNoteOn = 60; //middle C (C4)
//byte lastNoteOff = 60;

//int keyStatus[] = {noteC3act, noteG3act, noteA3act};

//-------MELODY #1---------//
boolean melody1Act = false;
byte melody1NoteLength = 7; //Number of notes in a melody - 1 (the highest number address of matrix addresses in an 8x1 matrix (0-7))


int noteJitter = 0; //'randomness' value of melody sequence playback (0 = 100% chance next note in melody sequence is played after the current note, 127 = equal probability of any other note from the sequence being played after the current note)

int melody1[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0 /*rest*/, NOTE_B3, NOTE_C4};

int melody1noteDurations[/*melody1NoteLength + 1*/] = {4, 8, 8, 4,4,4,4,4}; // note durations: 4 = quarter note, 8 = eighth note, etc.

void playMelody1()
{ 
  //tone(tonePin, melody1[notePosition]); //for debugging
   
  currentMillisTone = millis(); 

  if (notePosition == melody1NoteLength) //if we've reached the last note of the melody
  {
    int noteDuration = rate1/*rate1 or rate2*//melody1noteDurations[notePosition]; // to calculate the note duration, take one second divided by the note type. (e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.)
    int pauseBetweenNotes = noteDuration * 1.30;  // to distinguish the notes, set a minimum time between them. the note's duration + 30% seems to work well.   
    unsigned int timeElapsed = currentMillisTone - lastNoteOnMillis;
    int maxNoteEventLength = noteDuration + pauseBetweenNotes;

    byte randomNoteVal = map(noteJitter, 0, 127, 0, melody1NoteLength);
    byte randomNote = 1 + random(0, randomNoteVal);

    /////////
    /*
    if (debugCntr % 100 == 0)
    {
      Serial.print("notePosition = ");
      Serial.print(notePosition);
      Serial.print(" | currentMillisTone = ");
      Serial.print(currentMillisTone);
      Serial.print(" | lastNoteOnMillis = ");
      Serial.print(lastNoteOnMillis);
      Serial.print(" | timeElapsed = ");
      Serial.print(timeElapsed);
      Serial.print(" | randomNote = ");
      Serial.println(randomNote);
    }
    */
    /////////

    if (timeElapsed <= noteDuration)
    {
      noteIsOn = true;
      tone(tonePin, melody1[notePosition]);       
    }  
    else if (timeElapsed > noteDuration && timeElapsed <= maxNoteEventLength)
    {
      noteIsOn = false;
      noTone(tonePin); //silence for pause time 
      //tone(tonePin, 0, pauseBetweenNotes);  
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
    int noteDuration = rate1/melody1noteDurations[notePosition]; // to calculate the note duration, take a time value (2ms - ~10 seconds) divided by the note type. (e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.)
    int pauseBetweenNotes = noteDuration * 1.30 /*could replace this w/ variable value linked w/ encoder on CC76*/;  // to distinguish the notes, set a minimum time between them. the note's duration + 30% seems to work well.   
    unsigned int timeElapsed = currentMillisTone - lastNoteOnMillis;
    int maxNoteEventLength = noteDuration + pauseBetweenNotes;

    byte randomNoteVal = map(noteJitter, 0, 127, 0, melody1NoteLength);
    byte randomNote = 1 + random(0, randomNoteVal);

    ///////
    /*
    if (debugCntr % 100 == 0)
    {
      Serial.print("notePosition = ");
      Serial.print(notePosition);
      Serial.print(" | currentMillisTone = ");
      Serial.print(currentMillisTone);
      Serial.print(" | lastNoteOnMillis = ");
      Serial.print(lastNoteOnMillis);
      Serial.print(" | timeElapsed = ");
      Serial.print(timeElapsed);
      Serial.print(" | randomNote = ");
      Serial.println(randomNote);
    }
    */
    ///////
    
    if (timeElapsed <= noteDuration)
    {
      noteIsOn = true;
      tone(tonePin, melody1[notePosition]);       
    }  
    
    else if (timeElapsed > noteDuration && timeElapsed <= maxNoteEventLength)
    {
      noteIsOn = false;
      noTone(tonePin); //silence for pause time 
      //tone(tonePin, 0, pauseBetweenNotes);  
    }    
    
    else
    {
      lastNoteOnMillis = currentMillisTone;
      //notePosition++; //advance to the next note of the melody...
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




void playMelody2()
{ 
  currentMillisTone = millis();

  if (notePosition == melody2NoteLength) 
  {
    int noteDuration = rate1/*rate1 or rate2*//melody2noteDurations[notePosition]; // to calculate the note duration, take a time value (2ms - ~10 seconds) divided by the note type. (e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.)
    int pauseBetweenNotes = noteDuration * 1.30;  // to distinguish the notes, set a minimum time between them. the note's duration + 30% seems to work well.   
    unsigned int timeElapsed = currentMillisTone - lastNoteOnMillis;
    int maxNoteEventLength = noteDuration + pauseBetweenNotes;
    
    byte randomNoteVal = map(noteJitter, 0, 127, 0, melody2NoteLength);
    byte randomNote = 1 + random(0, randomNoteVal);
    
    if (timeElapsed <= noteDuration)
    {
      noteIsOn = true;
      tone(tonePin, melody2[notePosition]);       
    }  
    else if (timeElapsed > noteDuration && timeElapsed <= maxNoteEventLength)
    {
      noteIsOn = false;
      noTone(tonePin); //silence for pause time 
      //tone(tonePin, 0, pauseBetweenNotes);  
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
    int noteDuration = rate1/melody2noteDurations[notePosition]; // to calculate the note duration, take a time value (2ms - ~10 seconds) divided by the note type. (e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.)
    int pauseBetweenNotes = noteDuration * 1.30 /*could replace this w/ variable value linked w/ encoder on CC76*/;  // to distinguish the notes, set a minimum time between them. the note's duration + 30% seems to work well.   
    unsigned int timeElapsed = currentMillisTone - lastNoteOnMillis;
    int maxNoteEventLength = noteDuration + pauseBetweenNotes; //time of note duration and following rest (space between notes)
    
    ///
    byte randomNoteVal = map(noteJitter, 0, 127, 0, melody2NoteLength); //scale MIDI CC value range to the amount of addresses of the the melody2 matrix.
    byte randomNote = 1 + random(0, randomNoteVal); //pick a random note number (add 1 to always be able to advance melody even when noteJitter = 0.)
    ///
    
    if (timeElapsed <= noteDuration)
    {
      noteIsOn = true;
      tone(tonePin, melody2[notePosition]);       
    }  
    
    else if (timeElapsed > noteDuration && timeElapsed <= maxNoteEventLength)
    {
      noteIsOn = false;
      noTone(tonePin); //silence for pause time 
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



//=================ARDUINO SETUP FUNCTION=====================
void setup()  {
  MIDI.begin();
  Serial.begin(57600);
  MIDI.turnThruOff();
  MIDI.setInputChannel(MIDI_CHANNEL_OMNI); //think I need to do this so Arduino can work w/ any MIDI message Max will be sending it
  MIDI.setHandleControlChange(ControlViaCC);
  MIDI.setHandleNoteOn(NoteOnControl);
  MIDI.setHandleNoteOff(NoteOffControl); //only needed if receiving MIDI Note Off messages from controller device to stop notes
  
  activeLightPreset = 1;
    
  pinMode(redPin, OUTPUT); // declare pins to be an output:
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  pinMode(STBY, OUTPUT); //for motors
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  pinMode(ampSDNpin, OUTPUT); //for turning on/off amp
  digitalWrite(ampSDNpin, LOW); //turn amp off at startup
  
  armEEPROMwrite = false;
  readFromEEPROM();
  
  stopMotors();
} 

//================VOID LOOP FUNCTION======================

void loop()
{
  //debugCntr++;  //for debugging

  //=======BATTERY VOLTAGE MONITORING====
  pingBatVoltage(); //this seems like it might be causing issues with dropped messages, not sure if Arduino is able to send and receive MIDI messages at same time
                    //comment this out to ensure smoother operation for now...

  //===============MIDI READ AND HANDLING=============
  MIDI.read();

  //EEPROM Write functionality
  if (armEEPROMwrite == true && pShift == true)
    { 
      saveToEEPROM(lightPresetVal);
      armEEPROMwrite = false;
    }

  //PREDEFINED MELODY TRIGGERS
  if (melody1Act == false && melody2Act == false && keyModeAct == false)
  {
    noTone(tonePin);
    if (digitalRead(ampSDNpin) == HIGH) {digitalWrite(ampSDNpin, LOW);} //turn amp off if it isn't already, otherwise do nothing 
  }

  else if (melody1Act == true)
  {
    if (digitalRead(ampSDNpin) == LOW) {digitalWrite(ampSDNpin, HIGH);} //turn amp on if it isn't already, otherwise carry on  
    melody2Act = false; //I think this is needed to prevent melody performance conflicts if both melody trigger buttons are toggled 'on' at the same time...but will 1 loop of lag be audible?
    keyModeAct = false;
    playMelody1();
  }
  else if (melody2Act == true)
  {
    if (digitalRead(ampSDNpin) == LOW) {digitalWrite(ampSDNpin, HIGH);} //turn amp on if it isn't already, otherwise carry on
    melody1Act = false;
    keyModeAct = false;
    playMelody2();
  }
  else  //MANUAL MIDI NOTE PERFORMANCE
  {
    //Serial.println(keyStatus);
    if (digitalRead(ampSDNpin) == LOW) {digitalWrite(ampSDNpin, HIGH);} //turn amp on if it isn't already, otherwise carry on
    melody1Act = false;
    melody2Act = false;

    if (
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
        noteC4act
       )
    //if (keyStatus > 0) //if the keyStatus matrix has any value other than 0 in any of its addresses......
    {tone(tonePin, lastNoteOn);}
    
    else {noTone(tonePin);}
  }
  
  //===========LED LIGHT QUEUES==========   
  switch (queue)
  {   
    case SETCOLQ_CC:  //COLOR SET AND MANUAL FADE
        writeToLED(fdr1, fdr2, fdr3);               
      break;
      
    case FLASHQ_CC: //FLASH W/ colorJitter (randomness)...imported from ministage_complete_1_1 sketch.
      {
        if (melody1Act == true || melody2Act == true)
        {
          if (noteIsOn == true)
          {
            int32_t random_color = HSVtoRGB(random(0,360), 1, 1);
          
            float xr = colorJitter * r(random_color) + ((1 - colorJitter) * fdr1);
            float xg = colorJitter * g(random_color) + ((1 - colorJitter) * fdr2);
            float xb = colorJitter * b(random_color) + ((1 - colorJitter) * fdr3);

            LEDcolor = Color(max(min(xr,255),0) , max(min(xg, 255),0) , max(min(xb, 255), 0)); //excerpted from ministage code
            brightness = 1;

            AdjustBrightness(LEDcolor, brightness); 
            byte r = (LEDcolor & 0xFF0000) >> 16; //is this code necessary? I don't think it's doing what I think it's doing...TEST AND FIND OUT
            byte g = (LEDcolor & 0xFF00) >> 8;
            byte b = (LEDcolor & 0xFF);
    
            writeToLED(r, g, b);
          }

          else
          {
            writeToLED(0, 0, 0);
          }

        }

        else
        {
          currentMillis = millis(); //get the current time
          unsigned int timeElapsed = currentMillis - previousMillis; //get how much time has passed since we updated the animation
        
          if(timeElapsed > (rate1 / 8)) //check if we need to advance the state of the animation
          { 
            int32_t random_color = HSVtoRGB(random(0,360), 1, 1);
          
            float xr = colorJitter * r(random_color) + ((1 - colorJitter) * fdr1);
            float xg = colorJitter * g(random_color) + ((1 - colorJitter) * fdr2);
            float xb = colorJitter * b(random_color) + ((1 - colorJitter) * fdr3);
          
            LEDcolor = Color(max(min(xr,255),0) , max(min(xg, 255),0) , max(min(xb, 255), 0)); //excerpted from ministage code
            brightness = 1;
          
            altFlash();
          //go to the first state
            previousMillis = currentMillis; //set the last time we advanced the state of the animation
          }
        } 
      }
      break;
      
    case AUTOFADEQ_CC: //Color Fade Over Time
      {
        currentMillis = millis(); //get the current time
        unsigned int timeElapsed = currentMillis - previousMillis; //get how much time has passed since we updated the animation
        
        if(timeElapsed > rate1 / 16) //rate1 represents how much time must pass between updates of the animation - we divide it by 16 to get it in a good range
        {
          if(activeLightPreset >= lightPresetSelect + 1)
          {
            activeLightPreset = 1;
          }
          
          if(fadeTo(transColor, getActiveLightPreset()))
          {            
            activeLightPreset++;
          }
          //was working for 2 colors
          //fadeBetween(colorStore, transColor, lightPreset1); //original fadeBetween test
          previousMillis = currentMillis; //set the last time we advanced the state of the animation          
        }   
      }       
      break;
      
      
    case DYNAMICQ_CC: //Dynamic Pulse Control
      {        
        currentMillis = millis(); //get the current time
        unsigned int timeElapsedSinceLastDim = currentMillis - previousMillis;
             
        decay(timeElapsedSinceLastDim);    
        
        if (fdr4 >= 111) {brightness = 1;}
        if (fdr4 >= 95 && fdr4 <= 110) {brightness = .75;}               
        if (fdr4 >= 79 && fdr4 <= 94) {brightness = .6;}
        if (fdr4 >= 63 && fdr4 <= 78) {brightness = .5;}
        //if (fdr4 >= 55 && fdr4 <= 62) {brightness = .2;}
        //if (fdr4 >= 47 && fdr4 <= 54) {brightness = .1;}
        
        LEDcolor = Color(fdr1, fdr2, fdr3);
        dynamicPulse();     
        previousMillis = currentMillis;
        
      }      
      break;
      
    default:
      if (setColorAct == true) {queue = SETCOLQ_CC;}
      else {writeToLED(0, 0, 0);} 
  }
  
  //============MOTOR CONTROLS===========
  steerControl(steerDirection);
        //MOTOR A
  if (motorAon == true) {moveMotor(0, motorAspeed, motorAdirection);}
  else {moveMotor(0, 0, motorAdirection);}
        //MOTOR B
  if (motorBon == true) {moveMotor(1, motorBspeed, motorBdirection);}
  else {moveMotor(1, 0, motorBdirection);}
  
  
  
}

//==============MIDI NOTE CALL BACK================
void HandleNoteOn(byte channel, byte note, byte velocity);

void NoteOnControl (byte channel, byte note, byte velocity) 
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


void HandleNoteOff(byte channel, byte note, byte velocity);  //only needed if receiving MIDI Note Off messages from controller device to stop notes
                                                             //(TouchOSC does this, but Lemur and other controllers commonly send Note On messages w/ a 0 velocity value to turn notes off)
void NoteOffControl(byte channel, byte note, byte velocity)  //only needed if receiving MIDI Note Off messages from controller device to stop notes
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


//========== CC CALL BACK =============

void HandleControlChange (byte channel, byte number, byte value);

void ControlViaCC (byte channel, byte number, byte value)
{
  if (number == WRITECOLOR_CC) //=====USED TO BE PITCH SHIFT WHEEL WHEN WORKING W/ MINI STAGE=====
  {
    if (value > 120) {pShift = true;}
    else if (value < 26)
    {
     fdr1 = 0;
     fdr2 = 0;
     fdr3 = 0;
     pShift = false; 
    }
    else {pShift = false;}
  }
  
  //==========PRESSURE PADS======
  if (number == TRIGLP1_CC) 
  { 
    lightPresetVal = 1;
    lightPresetSelect = 1;
        
    if (pShift == false)
    {
    fdr1 = lightPreset1[0];
    fdr2 = lightPreset1[1];
    fdr3 = lightPreset1[2];
    }
    else
    {
    lightPreset1[0] = fdr1;
    lightPreset1[1] = fdr2;
    lightPreset1[2] = fdr3; 
    }
    
    transColor[0] = lightPreset1[0];
    transColor[1] = lightPreset1[1];
    transColor[2] = lightPreset1[2];
    
    colorStore[0] = lightPreset1[0];
    colorStore[1] = lightPreset1[1];
    colorStore[2] = lightPreset1[2];
  }
  
  if (number == TRIGLP2_CC) 
  { 
    lightPresetVal = 2;
    lightPresetSelect = 2;
     
    if (pShift == false)
    {
    fdr1 = lightPreset2[0];
    fdr2 = lightPreset2[1];
    fdr3 = lightPreset2[2];
    }
    else
    {
    lightPreset2[0] = fdr1;
    lightPreset2[1] = fdr2;
    lightPreset2[2] = fdr3; 
    }
    
    transColor[0] = lightPreset2[0];
    transColor[1] = lightPreset2[1];
    transColor[2] = lightPreset2[2];
    
    colorStore[0] = lightPreset2[0];
    colorStore[1] = lightPreset2[1];
    colorStore[2] = lightPreset2[2];
  }
  
  if (number == TRIGLP3_CC) 
  { 
    lightPresetVal = 3;
    lightPresetSelect = 3;
      
    if (pShift == false)
    {
    fdr1 = lightPreset3[0];
    fdr2 = lightPreset3[1];
    fdr3 = lightPreset3[2];
    }
    else
    {
    lightPreset3[0] = fdr1;
    lightPreset3[1] = fdr2;
    lightPreset3[2] = fdr3; 
    }
    
    transColor[0] = lightPreset3[0];
    transColor[1] = lightPreset3[1];
    transColor[2] = lightPreset3[2];
    
    colorStore[0] = lightPreset3[0];
    colorStore[1] = lightPreset3[1];
    colorStore[2] = lightPreset3[2];
  }
  
  if (number == TRIGLP4_CC) 
  { 
    lightPresetVal = 4;
    lightPresetSelect = 4;
        
    if (pShift == false)
    {
    fdr1 = lightPreset4[0];
    fdr2 = lightPreset4[1];
    fdr3 = lightPreset4[2];
    }
    else
    {
    lightPreset4[0] = fdr1;
    lightPreset4[1] = fdr2;
    lightPreset4[2] = fdr3; 
    }
    
    transColor[0] = lightPreset4[0];
    transColor[1] = lightPreset4[1];
    transColor[2] = lightPreset4[2];
    
    colorStore[0] = lightPreset4[0];
    colorStore[1] = lightPreset4[1];
    colorStore[2] = lightPreset4[2];
  }
  
  if (number == TRIGLP5_CC) 
  { 
    lightPresetVal = 5;
    lightPresetSelect = 5;
        
    if (pShift == false)
    {
    fdr1 = lightPreset5[0];
    fdr2 = lightPreset5[1];
    fdr3 = lightPreset5[2];
    }
    else
    {
    lightPreset5[0] = fdr1;
    lightPreset5[1] = fdr2;
    lightPreset5[2] = fdr3; 
    }
    
    transColor[0] = lightPreset5[0];
    transColor[1] = lightPreset5[1];
    transColor[2] = lightPreset5[2];
    
    colorStore[0] = lightPreset5[0];
    colorStore[1] = lightPreset5[1];
    colorStore[2] = lightPreset5[2];
  }
  
  if (number == TRIGLP6_CC)
  { 
    lightPresetVal = 6;
    lightPresetSelect = 6;
       
    if (pShift == false)
    {
    fdr1 = lightPreset6[0];
    fdr2 = lightPreset6[1];
    fdr3 = lightPreset6[2];
    }
    else
    {
    lightPreset6[0] = fdr1;
    lightPreset6[1] = fdr2;
    lightPreset6[2] = fdr3;
    }
    
    transColor[0] = lightPreset6[0];
    transColor[1] = lightPreset6[1];
    transColor[2] = lightPreset6[2];
    
    colorStore[0] = lightPreset6[0];
    colorStore[1] = lightPreset6[1];
    colorStore[2] = lightPreset6[2];
  }
  
  if (number == TRIGLP7_CC) 
  { 
    lightPresetVal = 7;
    lightPresetSelect = 7;
    
    if (pShift == false)
    {
    fdr1 = lightPreset7[0];
    fdr2 = lightPreset7[1];
    fdr3 = lightPreset7[2];
    }
    else
    {
    lightPreset7[0] = fdr1;
    lightPreset7[1] = fdr2;
    lightPreset7[2] = fdr3; 
    }
    
    transColor[0] = lightPreset7[0];
    transColor[1] = lightPreset7[1];
    transColor[2] = lightPreset7[2];
    
    colorStore[0] = lightPreset7[0];
    colorStore[1] = lightPreset7[1];
    colorStore[2] = lightPreset7[2];
  }
  
  if (number == TRIGLP8_CC) 
  { 
    lightPresetVal = 8;
    lightPresetSelect = 8;
        
    if (pShift == false)
    {
    fdr1 = lightPreset8[0];
    fdr2 = lightPreset8[1];
    fdr3 = lightPreset8[2];
    }
    else
    {
    lightPreset8[0] = fdr1;
    lightPreset8[1] = fdr2;
    lightPreset8[2] = fdr3; 
    }
    
    transColor[0] = lightPreset8[0];
    transColor[1] = lightPreset8[1];
    transColor[2] = lightPreset8[2];
    
    colorStore[0] = lightPreset8[0];
    colorStore[1] = lightPreset8[1];
    colorStore[2] = lightPreset8[2];
  }
  
  //==================LIGHT FADERS & QUEUE TOGGLE BUTTONS
  if (number == RED_CC)
  {
    fdr1 = smoothFade(value);
  }
  
  if (number == GREEN_CC)
  {
    fdr2 = smoothFade(value);
  }
  
  if (number == BLUE_CC)
  {
    fdr3 = smoothFade(value);
  }
  
  if (number == DYNAMIC_CC) //test signal for Dynamic Pulse light control
  {
    fdr4 = (value);
  }
  
  if (number == SETCOLQ_CC) //'SET COLOR' Light queue toggle button
  {
    if (value == 127)
    {
      setColorAct = true;
      queue = number;
    }
    else
    {
      setColorAct = false;
      queue = 0;
    }
  }

  if (number == FLASHQ_CC)
  {
    if (value == 127) {queue = number;}
    
    else
    {
      lightOnState = true;
      queue = 0;
    }
  }

  if (number == AUTOFADEQ_CC)
  {
    if (value == 127) {queue = number;}
    else
    {
      queue = 0;

      transColor[0] = getLightPresetPtr(lightPresetSelect)[0]; //this causes LED to always start at the last selected lightPreset color when starting up
      transColor[1] = getLightPresetPtr(lightPresetSelect)[1]; //the autoFade function. Otherwise the transColor never resets unless a new lightPreset is selected, which
      transColor[2] = getLightPresetPtr(lightPresetSelect)[2]; //causes the LED color to always pickup where it left off when starting and stopping the autoFade sequence w/out selecting a new lightPreset
      activeLightPreset = lightPresetSelect;                   //Now, multiple robots being commanded to initiate the autoFade function at the same time should remain in sync and always start at the same lightPreset


    }

  }

  if (number == DYNAMICQ_CC) //Light queue toggle buttons
  {
    if (value == 127) {queue = number;}
    else {queue = 0;}  
  }
  

  
  //============MOTOR FADERS & TOGGLE BUTTONS
  
  if (number == MSTRMTRSPD_CC) //Master Motor Speed Control for new steering system (D16 fader)
  {
    motorSpdVal = motorFader(value);
    if (value >= 80)
    {
      motorAdirection = 0; //should be clockwise wheel rotation..
      motorBdirection = 0;  
    }
    else
    {
      motorAdirection = 1; //should be counterclockwise wheel rotation...
      motorBdirection = 1;
    }
  }
  
  if (number == MTRDIRREV_CC) //ALL MOTORS DIRECTION REVERSE TOGGLE
  {
      motorAdirection = !motorAdirection;
      motorBdirection = !motorBdirection;
  }
  
  if (number == MTRACT_CC) //MOTOR ACTIVATE TOGGLE (both motors on/off)
  {
    if (value == 127)
    {
      motorAon = true;
      motorBon = true;
    }
    else
    {
      motorAon = false;
      motorBon = false;
    }
  }
  
  if (number == MTRSTEER_CC) //"Steering Wheel" Control (Rotary encoder B1 pretending to be rotary pot)
  {
    steerDirection = value;    
  }
  
  
  /////////////UNUSED FADERS

  
  /*if (number == 26) //(D15 fader)
  { 
    fdr7 = (value); 
  }*/

 
  /*if (number == 28) //(D17 fader) stepper motor control fader - forward / backward
  {
    fdr9 = (value);
  }*/
  

  
  
  //======Mod Wheel - Light Queue switcher
  
  if (number == 1) 
  {
   if (value >= 30 && value <= 115) {modWheel = 2;}
   else if (value >= 116) {modWheel = 3;}
   else {modWheel = 1;} 
  }
  
  //===EEPROM ARM BUTTON TOGGLE
  if (number == ARMEEPROM_CC)
  {
    if (value == 127) {armEEPROMwrite = true;}
    else {armEEPROMwrite = false;}
  }
  
  
  //============ ENCODERS ============
  
  if(number == RATE1_CC) //RATE #1  (knob pot 'B5' on axiom)
  {
    rate1 = mapRate1(value) * 2;
  }
  
  /*
  if (number == 76) //LIGHT PRESET SELECTOR (***REPLACE W/ RATE2 CONTROL, OR SOMETHING ELSE, OR DELETE OUTRIGHT***)
  { 
    byte thisValue = value + 1;
    lightPresetSelect = constrain(thisValue, 1, 8);
  }
  */
  
  if (number == FADESPD_CC) //PIXEL FADE/DECAY TIME
  {
    fadeSpeed = (value);
  } 
  
  if (number == JITTER_CC) //COLOR RANDOMNESS / JITTER CONTROL (knob B8)
  {
    colorJitter = map(value,0,127,0,1000)/1000.f;
    noteJitter = value;
  }
  
  //========MUSIC RELATED
  
  if(number == MEL1TRIG_CC)
  {
    if (value == 127) {melody1Act = true;}
    else
    {
      melody1Act = false;
      notePosition = 0;
    } 
  }
  
  if(number == MEL2TRIG_CC)
  {
     if (value == 127) {melody2Act = true;}
     else
     {
      melody2Act = false;
      notePosition = 0;
    } 
  }

  if (number == KEYACT_CC)
  {
    if (value == 127) {keyModeAct = true;}
    else {keyModeAct = false;}
  }
      
}


//============ CC CALL BACK HELPER FUNCTIONS

int mapRate1(int value) //Control for light speed / rate parameter settings - Knob B5
{
  int encdrVal;
  if (value >= 0 && value <= 42) {encdrVal = map(value, 0, 42, 1, 100);}
  else if (value >= 43 && value <= 84) {encdrVal = map(value, 43, 84, 101, 1000);}
  else {encdrVal = map(value, 85, 127, 1001, 5000);}
  return encdrVal;
}

/*
int mapRate2(int value) //Control for light speed / rate parameter settings - Knob B5
{
  int encdrVal;
  if (value >= 0 && value <= 42) {encdrVal = map(value, 0, 42, 1, 100);}
  else if (value >= 43 && value <= 84) {encdrVal = map(value, 43, 84, 101, 1000);}
  else {encdrVal = map(value, 85, 127, 1001, 5000);}
  return encdrVal;
}
*/

byte smoothFade(byte value) //smoother fader mapping for RGB faders
{
  int fdrval;
  if (value >= 0 && value <= 16)
    {fdrval = map(value, 0, 16, 0, 8);}
  else if (value >= 17 && value <= 100)
    {fdrval = map(value, 17, 100, 9, 200);}
  else {fdrval = map(value, 101, 127, 201, 255);}
  return fdrval;
}

byte motorFader(byte value)
{
  byte fdrval;
  
  if (value >= 80)
    {fdrval = map(value, 80, 127, 1, 255);}
  else if (value <= 48)
    {fdrval = map(value, 48, 0, 1, 255);}
  else {fdrval = 0;}
  
  return fdrval;
}

/*************BATTERY MONITOR FUNCTIONS **************/

void MIDICC_TX(byte CC, byte value)
{
  Serial.flush();
  byte MIDICCmessage[3] = {176, CC, value}; //try changing byte to int
    
  Serial.write(MIDICCmessage, 3);
  
  //Serial.write(177); //Byte 1: message byte of MIDI CC message (1011CCCC... 10110001 = MIDI CC on channel 1)
  //Serial.write(CC); //Byte 2: Control number
  //Serial.write(value); //Byte 3: Value number
  
}


void pingBatVoltage()
{
  currentMillis = millis();
  unsigned int timeElapsed = currentMillis - previousBatPingMillis;

  if (timeElapsed >= 5000 && MIDI.read() == false)
  {
    //Read voltage at analog pins....
    bat1MonPinVal = analogRead(bat1MonPin);  // read the voltage on battery 1 divider
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
        
    MIDICC_TX(BAT1VOLTREAD_CC, bat1VoltageCCVal);
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
    previousBatPingMillis = currentMillis;
  }
}

/************* LED COLOR CONTROL FUNCTIONS ************/
void writeToLED(byte red, byte green, byte blue)
{ 
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

/*
void flash(byte redBrightness, byte greenBrightness, byte blueBrightness)
{  
  if (lightOnState == true) {writeToLED(redBrightness, greenBrightness, blueBrightness);}  
  else {writeToLED(0, 0, 0);}
  
  lightOnState = !lightOnState;   
}
*/

void altFlash()
{
  if (lightOnState == true)
  {
    AdjustBrightness(LEDcolor, brightness); 
    byte r = (LEDcolor & 0xFF0000) >> 16; //is this code necessary? I don't think it's doing what I think it's doing...TEST AND FIND OUT
    byte g = (LEDcolor & 0xFF00) >> 8;
    byte b = (LEDcolor & 0xFF);
    
    writeToLED(r, g, b);
  }
  
  else {writeToLED(0, 0, 0);}
  
  lightOnState = !lightOnState;
  
}

void dynamicPulse()
{       
    uint32_t thisLEDColor = AdjustBrightness(LEDcolor, brightness);
    
    byte red = r(thisLEDColor);
    byte grn = g(thisLEDColor);
    byte blu = b(thisLEDColor);    
    
    writeToLED(red, grn, blu);
}

//vvvvvv=======Functions Matt wrote...used for fading from mini stage code========vvvvv

uint32_t AdjustBrightness(uint32_t color, float brightness)
{
   if(brightness < 0)
    brightness = 0;
    
   byte r = (color & 0xFF0000) >> 16;
   byte g = (color & 0xFF00) >> 8;
   byte b = (color & 0xFF);
   r = r * brightness;
   g = g * brightness;
   b = b * brightness;
   return Color(r,g,b);
}

float getBrightness(float Time, float ActivationTime, float FadeTime)
{
  if(Time < ActivationTime)
    return 0.0f;
  if(Time > ActivationTime + FadeTime)
    return 0.0f;
  float b = 1 - ((Time - ActivationTime) / (FadeTime)); 
  if(b < 0)
    b = 0;
  return b;
}

float getActivateTime(int pixelIndex, float secondsPerCycle, int numberOfPixels)
{  
  return pixelIndex * (secondsPerCycle / ((float)numberOfPixels));
}

//===========^^^^^^^^^^^^^^^^^^^^^^==========

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

int calculateStep(int prevValue, int endValue)
{
  int stepSize = endValue - prevValue;
  return stepSize;
}

int calculateVal(int stepSize, int val)
{
  if (stepSize) // If step is non-zero
  { 
    if (stepSize > 0) {val += 1;}  //increment the value if step is positive...             
    else if (stepSize < 0) {val -= 1;}  //...or decrement it if step is negative           
  }
  // Defensive driving: make sure val stays in the range 0-255
  if (val > 255) {val = 255;} 
  else if (val < 0) {val = 0;}
  return val;
}

void crossFade(int color1[3], int color2[3])
{
  //writeToLED....SHOW color1 first!
  writeToLED(color1[0], color1[1], color1[2]);
  
  //then calculate distance between color1 and color2  
  int stepR = calculateStep(color1[0], color2[0]);
  int stepG = calculateStep(color1[1], color2[1]); 
  int stepB = calculateStep(color1[2], color2[2]);
  
  //then increment color1 RGB values '1' closer to color2 values
  transColor[0] = calculateVal(stepR, color1[0]);
  transColor[1] = calculateVal(stepG, color1[1]);
  transColor[2] = calculateVal(stepB, color1[2]);
}

//current_color is transColor
bool fadeTo(int current_color[3], int target_color[3])
{  
  if( current_color[0] == target_color[0] &&
  current_color[1] == target_color[1] &&
  current_color[2] == target_color[2])
    return true;
  
  crossFade(current_color, target_color);
  return false;
}


void fadeBetween(int colorStore[3], int transColor[3], int color2[3]) // ***NOT USING THIS FUNCTION CURRENTLY...BUT MAY LIKE TO IN FUTURE** ORIGINAL fadeBetween function that works w/ lightPreset array input for third parameter
{
 
 int thisColorStore = Color(colorStore[0], colorStore[1], colorStore[2]);
 int thisTransColor = Color(transColor[0], transColor[1], transColor[2]);
 int thisColor2 = Color(color2[0], color2[1], color2[2]);
 
 if (thisTransColor == thisColorStore) {colorStoreReached = true;}
 if (thisTransColor == thisColor2) {colorStoreReached = false;}
  
 if (colorStoreReached == true) {crossFade(transColor, color2);}
 else {crossFade(transColor, colorStore);}

}


/**************DC MOTOR CONTROL FUNCTIONS***************/
void moveMotor(int motor, int motorSpeed, int motorDirection)
{
//Move specific motor at speed and direction
//motor: 1 for B, 0 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(motorDirection == 1)
  {
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if(motor == 0)
  {
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, motorSpeed);
  }
  else
  {
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, motorSpeed);
  }
}

void stopMotors()
{
//enable standby  
  digitalWrite(STBY, LOW); 
}

void steerControl(int value)
{
  /*
  Serial.print("MotorASpd: ");
  Serial.print(motorAspeed);
  Serial.print(" / MotorBSpd: ");
  Serial.print(motorBspeed);
  Serial.print(" / Steer Val: ");
  Serial.print(value);
  Serial.print(" / MtrADr: ");
  Serial.print(motorAdirection);
  Serial.print(" / MtrBDr: ");
  Serial.println(motorBdirection);
  */
  
 //Straight Ahead
    if ( value >= 59 && value <= 69)
    {
      motorAspeed = motorSpdVal;
      motorBspeed = motorSpdVal;
      motorAdirection = motorBdirection;
    }
    
    //Turning to the right
    else if (value >= 70 && value <= 122)
    {
     motorBdirection = motorAdirection;
     
     int rightWheelScalingRightT = map(value, 70, 122, 1, motorAspeed);
     
     motorAspeed = motorSpdVal;  //Left wheel
     
     motorBspeed = motorAspeed - rightWheelScalingRightT; //Right wheel...NOTE: may want to #define or create 'lowSpeedThres' variable for 4th parameter in preceding map function
     if (motorBspeed <= 0) {motorBspeed = 0;} //lowest speed control threshold, motorBspeed will never be less than 0     
    }
    
    //Hard right / Clockwise spinning
    else if (value >= 123)
    {
      motorAspeed = motorSpdVal;
      motorBdirection = !motorAdirection; //will this cause issues when trying to adjust the motorSpdVal while spinning? Try and find out...
      motorBspeed = motorSpdVal;      
    }
    
    //Turning to the left
    else if(value >= 5 && value <= 58)
    {
     motorAdirection = motorBdirection; 
      
     int leftWheelScalingLeftT = map(value, 58, 5, 1, motorBspeed);
      
     motorBspeed = motorSpdVal;  //Right wheel..NOT WORKING
     
     motorAspeed = motorBspeed - leftWheelScalingLeftT; //Left wheel...NOTE: may want to #define or create 'lowSpeedThres' variable for 4th parameter in preceding map function
     if (motorAspeed <= 0) {motorAspeed = 0;} //lowest speed control threshold, motorAspeed will never be less than 0 
    }
    
    //Hard left / Counter-Clockwise spinning
    else //(value <=4)
    {
      motorBspeed = motorSpdVal;
      motorAdirection = !motorBdirection; //will this cause issues when trying to adjust the motorSpdVal while spinning? Try and find out...
      motorAspeed = motorSpdVal;      
    } 
}


# TODOS

Jim

- Refactor NoteControl.cpp to accept all notes
- Get Lemur, test it out
    + See if I can get it talking to Node
    + Scott send a copy of the Lemur Template
    + Hairless Midi
- Abstract out EEPROM
    + Think about saving arbitrary melodies - 4x16
- Leave Melodies alone -- eventually we'll just record melodies
- Make Flags Abstract, use a map

Scott:

- Working on board stuff
- Looking into sequencer template
    + Programming a melody -- how to represent?
- Looking at pre-programmed sequences in Max

Future:

- Start looking at modeling things in software



# Mini Model Code and CC Reference Notes:

MIDI CC | Functionality | Associated Variable Name(s)
------------- | ------------- | ------------- 
20 | ‘Red’ color level | fdr1, RED_CC
21 | ‘Green’ color level | fdr2, GREEN_CC
22 | ‘Blue’ color level | fdr3, BLUE_CC
23 |  test signal for Dynamic Pulse light control | fdr4, DYNAMIC_CC
52 |  Toggles ‘Set Color’ light mode | queue, qval
53 |  Toggles ‘Flash’ light mode | queue, qval
54 | Toggles ‘AutoFade’ light mode | queue, qval
55 | Toggles ‘Dynamic Pulse’ light mode | queue, qval
90 | Mode shifter | Used to save light Presets temporarily and to EEPROM. (if value == 127). also used to clear lightPreset value (turn off LED) (if value <=20). Originally used w/ pitch shift wheel :: (consider changing variable name), WRITECOLOR_CC
80 | Trigger lightPreset | originally used w/ pressure pad C23, lightPreset1
81 | Trigger lightPreset2 | originally used w/ pressure pad C24, lightPreset2
82 | Trigger lightPreset3 | originally used w/ pressure pad C25, lightPreset3
83 | Trigger lightPreset4 | originally used w/ pressure pad C26, lightPreset4
84 | Trigger lightPreset5 | originally used w/ pressure pad C27, lightPreset5
85 | Trigger lightPreset6 | originally used w/ pressure pad C28, lightPreset6
86 | Trigger lightPreset7 | originally used w/ pressure pad C29, lightPreset7
87 | Trigger lightPreset8 | originally used w/ pressure pad C30, lightPreset8
27 | Master motor speed control | motorSpdVal
58 | Motor reverse direction toggle | motorAdirection, motorBdirection
59 | Motor activate toggle | motorAon, motorBon
71 | Steering control | steerDirection
60 | arm EEPROM write toggle | armEEPROMwrite
1 | Light mode shifter | currently unused, previously associated w/ mod wheel (modWheel)
75 | Rate #1 encoder value | rate1, RATE1_CC
76 |  light preset selector | currently unused, lightPresetSelect
77 | fade (decay) speed encoder value | fadeSpeed, FADESPD_CC
78 | randomness encoder value | colorJitter, noteJitter, JITTER_CC
30 | battery 1 (arduino battery) voltage reading |
31 | battery 2 (motor battery) voltage reading | no longer using 2nd battery, CC currently unused
28 | trigger melody 1 | melody1Act
29 | trigger melody 2 | melody2Act
35 | activate keyboard control mode | keyModeAct

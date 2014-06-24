

Mini Model Code and CC Reference Notes:

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
90 | Mode shifter | • used to save light Presets temporarily and to EEPROM <br/>
(if value == 127)<br/>
• also used to clear lightPreset value (turn off LED)<br/>
(if value <=20)<br/>
• originally used w/ pitch shift wheel<br/>
pShift (consider changing variable name), WRITECOLOR_CC<br/>

etc. finish 
80
Trigger lightPreset1
• originally used w/ pressure pad C23
lightPreset1
lightPresetVal
lightPresetSelect
81
Trigger lightPreset2
• originally used w/ pressure pad C24
lightPreset2
lightPresetVal
lightPresetSelect
82
Trigger lightPreset3
• originally used w/ pressure pad C25
lightPreset3
lightPresetVal
lightPresetSelect
83
Trigger lightPreset4
• originally used w/ pressure pad C26
lightPreset4
lightPresetVal
lightPresetSelect
84
Trigger lightPreset5
• originally used w/ pressure pad C27
lightPreset5
lightPresetVal
lightPresetSelect
85
Trigger lightPreset6
• originally used w/ pressure pad C28
lightPreset6
lightPresetVal
lightPresetSelect
86
Trigger lightPreset7
• originally used w/ pressure pad C29
lightPreset7
lightPresetVal
lightPresetSelect
87
Trigger lightPreset8
• originally used w/ pressure pad C30
lightPreset8
lightPresetVal
lightPresetSelect
27
Master motor speed control
motorSpdVal
58
Motor reverse direction toggle
motorAdirection
motorBdirection
59
Motor activate toggle
motorAon
motorBon
71
Steering control
steerDirection
60
arm EEPROM write toggle
armEEPROMwrite
1
Light mode shifter
• currently unused
• previously associated w/ mod wheel
modWheel
75
Rate #1 encoder value
rate1, RATE1_CC
76
light preset selector
• currently unused
lightPresetSelect
77
fade (decay) speed encoder value
fadeSpeed, FADESPD_CC
78
randomness encoder value
colorJitter, noteJitter, JITTER_CC
30
battery 1 (arduino battery) voltage reading

31
battery 2 (motor battery) voltage reading
• no longer using 2nd battery, CC currently unused

28
trigger melody 1
melody1Act
29
trigger melody 2
melody2Act
35
activate keyboard control mode
keyModeAct

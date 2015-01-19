#include "melodyian.h"

namespace Flags {

  bool melody1Act = false;
  bool melody2Act = false;
  bool keyModeOn = false;  //adding for LED flashes in FLASH mode queue when keyMode is active and noteIsOn == true
  bool noteIsOn = false;
  bool MIDInotePanicAct = false; //added for new MIDI note panic feature

  bool melodyOneAct() {return melody1Act;}

  bool melodyTwoAct() {return melody2Act;}

  bool keyModeAct() {return keyModeOn;}    //adding for LED flashes in FLASH mode queue when keyMode is active and noteIsOn == true

  bool noteOn() {return noteIsOn;}

  bool MIDInotePanic() {return MIDInotePanicAct;}

  void setMelodyOne(bool val) {melody1Act = val;}

  void setMelodyTwo(bool val) {melody2Act = val;}

  void setKeyMode(bool val) {keyModeOn = val;}  //adding for LED flashes in FLASH mode queue when keyMode is active and noteIsOn == true

  void setNoteOn(bool val) {noteIsOn = val;}

  void setMIDInotePanic(bool val) {MIDInotePanicAct = val;} //added for new MIDI note panic feature

}

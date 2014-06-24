#include "melodyian.h"

namespace Flags {

  bool melody1Act = false;
  bool melody2Act = false;
  bool noteIsOn = false;

  bool melodyOneAct() {
    return melody1Act;
  }

  bool melodyTwoAct() {
    return melody2Act;
  }

  bool noteOn() {
    return noteIsOn;
  }

  void setMelodyOne(bool val) {
    melody1Act = val;
  }

  void setMelodyTwo(bool val) {
    melody2Act = val;
  }

  void setNoteOn(bool val) {
    noteIsOn = val;
  }

}

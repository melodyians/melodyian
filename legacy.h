#include "types.h"
#include "components.h"

namespace Motor {
  void processMotorCC (byte channel, byte number, byte value);
  void actuateMotors(HardwareInterface * hardware);
}
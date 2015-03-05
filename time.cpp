#include "time.h"
#include <Arduino.h>

Timer::Timer () {
  start_ts = millis();
  current_ts = start_ts;
  previous_ts = start_ts;
}

unsigned short Timer::step () {
  previous_ts = current_ts;
  current_ts = millis();
  return current_ts - previous_ts;
}

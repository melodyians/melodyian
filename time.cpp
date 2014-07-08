#include "time.h"
#include <Arduino.h>

Timer::Timer () {
  start_ts = millis();
  current_ts = start_ts;
  previous_ts = start_ts;
  dt = 0;
}

unsigned long Timer::step () {
  previous_ts = current_ts;
  current_ts = millis();
  dt = current_ts - previous_ts;
  return dt;
}

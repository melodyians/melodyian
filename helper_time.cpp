#include <Arduino.h>
#include "time.h"

Timer::Timer () {
  this->start_ts = millis();
  this->current_ts = this->start_ts;
  this->previous_ts = this->start_ts;
}

unsigned short Timer::step () {
  this->previous_ts = this->current_ts;
  this->current_ts = millis();
  return this->current_ts - this->previous_ts;
}

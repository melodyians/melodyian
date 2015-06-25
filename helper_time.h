#pragma once

class Timer {

  public:
    Timer();
    unsigned short step();

  private:
    unsigned long start_ts;
    unsigned long current_ts;
    unsigned long previous_ts;
};
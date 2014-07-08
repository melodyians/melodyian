
class Timer {

  public:
    Timer();
    unsigned long step();

  private:
    unsigned long start_ts;
    unsigned long current_ts;
    unsigned long previous_ts;
    unsigned long dt;
};

/*
class CountUp {

  public:
    CountUp();
    void step(unsigned long);
    bool completed();
    bool resetCount(unsigned long);

  private:
    unsigned long count_length;
    unsigned long time_so_far;

}
*/

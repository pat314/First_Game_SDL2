
#ifndef TIMER_H_
#define TIMER_H_

class Timer
{
public:
    Timer();
    ~Timer();

    void start();
    void stop();
    void paused();
    void resume();

    int get_tick();

    bool isStarted();
    bool isPaused();


private:
    int start_tick;
    int pause_tick;

    bool is_paused;
    bool is_started;


};

#endif // TIMER_H_

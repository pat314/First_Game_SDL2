#include "Timer.h"
#include "CommonFunc.h"


Timer::Timer()
{
    start_tick = 0;
    pause_tick = 0;
    is_paused = false;
    is_started = false;
}

Timer::~Timer()
{

}

//hàm lấy thời điểm thực hiện khung hình thứ n
void Timer::start()
{
    is_started = true;
    is_paused = false;
    //Lấy thời điểm bắt đầu
    start_tick = SDL_GetTicks();
}

void Timer::stop()
{
    is_paused = false;
    is_started = false;
}

void Timer::paused()
{
    if (is_started = true && is_paused == false)
    {
        is_paused = true;
        //Thời gian tạm dừng bằng thời gian đã trôi qua trừ đi thời gian đã trôi qua khi game chạy
        pause_tick = SDL_GetTicks() - start_tick;
    }
}

void Timer::resume()
{
    if (is_paused == true)
    {
        is_paused = false;
        //Thời điểm cuối cùng còn chưa pause
        start_tick = SDL_GetTicks() - pause_tick;
        pause_tick = 0;
    }
}

//Hàm lấy khoảng thời gian thực hiện khung hình thứ n
int Timer::get_tick()
{
    if (is_started == true)
    {
        if (is_paused == true) return pause_tick;
        else return SDL_GetTicks() - start_tick;
    }
    return 0;
}

bool Timer::isStarted()
{
    return is_started;
}

bool Timer::isPaused()
{
    return is_paused;
}

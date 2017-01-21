#include "SV_Timer.h"


template <class time_type>
SV_Timer::SV_Timer(time_type interval) {
    this->interval = interval;
    auto start = Clock::now();
}


void SV_Timer::restart() {
    start = Clock::now();
}


template <class time_type>
bool SV_Timer::is_done() {
    auto elapsed = std::chrono::duration_cast<time_type>(Clock::now() - start).count();
    return elapsed >= interval;
}

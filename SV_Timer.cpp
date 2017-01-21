#include "SV_Timer.h"


template <class time_type>
SV_Timer<time_type>::SV_Timer(time_type interval) {
    this->interval = interval;
    start = Clock::now();
}


template <class time_type>
void SV_Timer<time_type>::restart() {
    start = Clock::now();
}


template <class time_type>
bool SV_Timer<time_type>::is_done() {
    auto elapsed = std::chrono::duration_cast<time_type>(Clock::now()-start);
    return elapsed >= interval;
}


template class SV_Timer<std::chrono::nanoseconds>;
template class SV_Timer<std::chrono::microseconds>;
template class SV_Timer<std::chrono::milliseconds>;
template class SV_Timer<std::chrono::seconds>;

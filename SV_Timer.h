#ifndef SANICVIEW_SV_TIMER_H
#define SANICVIEW_SV_TIMER_H

#include <chrono>

template <class time_type>
class SV_Timer {
public:
    SV_Timer(time_type interval);
    void restart();
    bool is_done();
private:
    std::chrono::high_resolution_clock::time_point start;
    time_type interval;
    typedef std::chrono::high_resolution_clock Clock;
};


#endif //SANICVIEW_SV_TIMER_H

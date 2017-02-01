#ifndef SANICVIEW_SV_EVENT_H
#define SANICVIEW_SV_EVENT_H

#include <xcb/xcb.h>

enum SV_event_type {expose,
    mouse_push, mouse_move, mouse_release, mouse_enter, mouse_leave,
    key_press, key_release};


class SV_Event {
public:
    SV_Event(const xcb_generic_event_t* event);
    SV_event_type type() const;
    int time() const;
    int x() const;
    int y() const;
private:
    int event_x = -1, event_y = -1;
    int event_time = 0;
    SV_event_type event_type;
    template <typename event_type>
    void set_event_data(event_type evt) {
        event_x = evt->event_x;
        event_y = evt->event_y;
        event_time = evt->time;
    }
};


#endif //SANICVIEW_SV_EVENT_H

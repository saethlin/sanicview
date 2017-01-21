#ifndef SANICVIEW_SV_EVENT_H
#define SANICVIEW_SV_EVENT_H

#include <xcb/xcb.h>

class SV_Event {
public:
    SV_Event(xcb_generic_event_t* event);
    SV_event_type type();
    int time();
    int event_x();
    int event_y();
private:

};


enum SV_event_type {expose,
                    mouse_push, mouse_move, mouse_release, mouse_enter, mouse_leave,
                    key_press, key_release};


#endif //SANICVIEW_SV_EVENT_H

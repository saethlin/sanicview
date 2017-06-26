#include "SV_Event.h"
#include <xcb/xcb.h>


SV_Event::SV_Event(const xcb_generic_event_t *event) {
    switch (event->response_type) {
        case XCB_EXPOSE: {
            event_type = expose;
            break;
        }
        case XCB_BUTTON_PRESS: {
            event_type = mouse_push;
            set_event_data((xcb_button_press_event_t*)event);
            break;
        }
        case XCB_BUTTON_RELEASE: {
            event_type = mouse_release;
            set_event_data((xcb_button_release_event_t*)event);
            break;
        }
        case XCB_MOTION_NOTIFY: {
            event_type = mouse_move;
            set_event_data((xcb_motion_notify_event_t*)event);
            break;
        }
        case XCB_ENTER_NOTIFY: {
            event_type = mouse_enter;
            set_event_data((xcb_enter_notify_event_t*)event);
            break;
        }
        case XCB_LEAVE_NOTIFY: {
            event_type = mouse_leave;
            set_event_data((xcb_leave_notify_event_t*)event);
            break;
        }
        case XCB_KEY_PRESS: {
            event_type = key_press;
            set_event_data((xcb_key_press_event_t*)event);
            break;
        }
        case XCB_KEY_RELEASE: {
            event_type = key_release;
            set_event_data((xcb_key_release_event_t*)event);
            break;
        }
        default:
            break;
    }
}


SV_event_type SV_Event::type() const {return event_type;}


int SV_Event::time() const {return event_time;}


int SV_Event::x() const {return event_x;}


int SV_Event::y() const {return event_y;}


int SV_Event::key() const {return event_key;}

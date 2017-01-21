#include <xcb/xcb.h>
#include "SV_Event.h"


SV_Event::SV_Event(xcb_generic_event_t *event) {
    switch (event->response_type) {
        case XCB_EXPOSE: {
            type = expose;
            break;
        }
        case XCB_BUTTON_PRESS: {
            type = mouse_push;
            break;
        }
        case XCB_BUTTON_RELEASE: {
            type = mouse_release;
            break;
        }
        case XCB_MOTION_NOTIFY: {
            type = mouse_move;
            break;
        }
        case XCB_ENTER_NOTIFY: {
            type = mouse_enter;
            break;
        }
        case XCB_LEAVE_NOTIFY: {
            type = mouse_leave;
            break;
        }
        case XCB_KEY_PRESS: {
            type = key_press;
            break;
        }
        case XCB_KEY_RELEASE: {
            type = key_release;
            break;
        }
        default:
            break;
    }
}


int SV_Event::get_time() {return time;}


int SV_Event::event_x() {return x;}


int SV_Event::event_y() {return y;}
#include <algorithm>
#include "SV_Window.h"
#include "SV_Widget.h"
#include <iostream>


SV_Window::SV_Window(int width, int height) {
    this->width = width;
    this->height = height;
    /* Open the connection to the X server */
    connection = xcb_connect(NULL, NULL);

    /* Get the first screen */
    xcb_screen_t* screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;

    /* Create black (foreground) graphic context */
    xcb_window = screen->root;
    foreground = xcb_generate_id(connection);
    uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES ;
    uint32_t values[2] = {screen->black_pixel, 0};

    xcb_create_gc(connection, foreground, xcb_window, mask, values);

    /* Create a window */
    xcb_window = xcb_generate_id(connection);

    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    values[0] = screen->black_pixel;
    values[1] = (XCB_EVENT_MASK_EXPOSURE |
                 XCB_EVENT_MASK_BUTTON_PRESS |
                 XCB_EVENT_MASK_BUTTON_RELEASE |
                 XCB_EVENT_MASK_POINTER_MOTION |
                 XCB_EVENT_MASK_BUTTON_MOTION |
                 XCB_EVENT_MASK_KEY_PRESS |
                 XCB_EVENT_MASK_KEY_RELEASE);

    xcb_create_window(connection,                    /* connection          */
                      XCB_COPY_FROM_PARENT,          /* depth               */
                      xcb_window,                    /* window Id           */
                      screen->root,                  /* parent window       */
                      0, 0,                          /* x, y                */
                      width, height,                 /* width, height       */
                      10,                            /* border_width        */
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
                      screen->root_visual,           /* visual              */
                      mask, values);                 /* masks */

    drawing_buffer = SV_PixelTable(screen->width_in_pixels, screen->height_in_pixels);
}


void SV_Window::run() {
    // Map the window on the screen and flush
    xcb_map_window(connection, xcb_window);
    xcb_flush(connection);

    xcb_generic_event_t* xcb_event_ptr;
    while ((xcb_event_ptr = xcb_wait_for_event(connection))) {
        auto event = SV_Event(xcb_event_ptr);
            for (const auto &widget : widgets) {
                if (widget->x() < event.x() and event.x() < widget->x() + widget->w() and
                    widget->y() < event.y() and event.y() < widget->y() + widget->h()) {
                    if (widget->handle(event)) {break;}
                }
            }
        if (timer.is_done() or event.type() == expose) {
            timer.restart();
            for (const auto& widget : widgets) {
                if (widget->needsdraw()) {
                    widget->draw();
                }
            }
            flush();
        }
        free(xcb_event_ptr);
    }
}


void SV_Window::add(SV_Widget* widget) {
    this->widgets.push_back(widget);
}


/*
 * Get the changed pixels from the buffer, sort them,
 * and draw to the screen in groups of the same color
 */
void SV_Window::flush() {
    if (drawing_buffer.empty()) {return;}

    auto changed_pixels = drawing_buffer.get_changed();
    auto current_color = changed_pixels.front().color;

    std::sort(changed_pixels.begin(), changed_pixels.end());

    for (const auto& px : changed_pixels) {
        if (px.color != current_color) {
            xcb_change_gc(connection, foreground, XCB_GC_FOREGROUND, &current_color);
            xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, xcb_window, foreground, color_run.size(), color_run.data());
            color_run.clear();
            current_color = px.color;
        }
        color_run.push_back({(int16_t)px.x, (int16_t)px.y});
    }

    xcb_change_gc(connection, foreground, XCB_GC_FOREGROUND, &current_color);
    xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, xcb_window, foreground, color_run.size(), color_run.data());
    color_run.clear();

    drawing_buffer.clear();
    xcb_flush(connection);
}


void SV_Window::draw_point(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    drawing_buffer.insert(x, y, r, g, b);
}

SV_Window::~SV_Window() {
    xcb_disconnect(connection);
}
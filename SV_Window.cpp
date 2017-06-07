#include "SV_Window.h"
#include "SV_Widget.h"
#include "SV_PixelTable.h"
#include <algorithm>
#include <iostream>


SV_Window::SV_Window(int width, int height, int framerate) {
    this->width = width;
    this->height = height;
    this->framerate = std::chrono::duration<float, std::milli>(1/(double)framerate);
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

    // Prevent resizing
    xcb_size_hints_t hints;
    xcb_icccm_size_hints_set_min_size(&hints, width, height);
    xcb_icccm_size_hints_set_max_size(&hints, width, height);
    xcb_icccm_set_wm_size_hints(connection, xcb_window, XCB_ATOM_WM_NORMAL_HINTS, &hints);

    drawing_buffer = SV_PixelTable(width, height);
}


void SV_Window::draw_loop() {
    { // Special scope for the lock guard
        std::lock_guard<std::mutex> guard(lock);
        for (const auto& widget : widgets) {
            if (widget->needsdraw()) {
                widget->draw();
            }
        }
        flush();
    }
    std::this_thread::sleep_for(framerate);
    thread_alive = false;
}


void SV_Window::run() {
    // Map the window on the screen and flush
    xcb_map_window(connection, xcb_window);
    xcb_flush(connection);

    bool was_handled = false;
    SV_Widget* has_mouse = NULL;
    xcb_generic_event_t* xcb_event_ptr;
    while ((xcb_event_ptr = xcb_wait_for_event(connection))) {
        SV_Event event(xcb_event_ptr);
        lock.lock();
        // Special case for mouse events; redirect them to any widget that has grabbed the mouse focus
        if (has_mouse && (event.type() == mouse_move || event.type() == mouse_release)) {
            has_mouse->handle(event);
            was_handled = true;
            if (event.type() == mouse_release) {
                has_mouse = NULL;
            }
        }
        else {
            for (const auto& widget : widgets) {
                if (event.type() == key_press || (widget->x() < event.x() && event.x() < widget->x() + widget->w() &&
                     widget->y() < event.y() && event.y() < widget->y() + widget->h())) {
                    was_handled = widget->handle(event);
                    if (was_handled && event.type() == mouse_push) {
                        has_mouse = widget;
                    }
                    if (was_handled) {break;}
                }
            }
        }

        // Clean up and close if esc is pressed
        if (event.type() == key_press) {
            if ((int)((xcb_key_press_event_t*)xcb_event_ptr)->detail == 9) {
                free(xcb_event_ptr);
                lock.unlock();
                return;
            }
        }

        // If no thread is alive, launch one to check if any widgets needs to be redrawn
        if ((!thread_alive and was_handled) or event.type() == expose) {
            thread_alive = true;
            std::thread draw_thread(&SV_Window::draw_loop, this);
            draw_thread.detach();
        }
        was_handled = false;
        free(xcb_event_ptr);
        lock.unlock();
    }
}


void SV_Window::add(SV_Widget* widget) {
    this->widgets.push_back(widget);
}


void SV_Window::flush() {
    if (drawing_buffer.empty()) {return;}

    auto& changed_pixels = drawing_buffer.get_changed();
    if (is_first_draw) {
        changed_pixels.erase(std::remove_if(changed_pixels.begin(), changed_pixels.end(), [](xcb_pixel p){return p.color == 0;}), changed_pixels.end());
    }
    auto current_color = changed_pixels.front().color;

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

    xcb_flush(connection);
    is_first_draw = false;
}


void SV_Window::draw_point(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    drawing_buffer.insert(x, y, r, g, b);
}


SV_Window::~SV_Window() {xcb_disconnect(connection);}
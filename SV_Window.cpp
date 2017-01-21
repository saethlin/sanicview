#include "SV_Window.h"
#include "SV_Widget.h"
#include <iostream>


SV_Window::SV_Window(int width, int height) {
    this->width = width;
    this->height = height;
    /* Open the connection to the X server */
    connection = xcb_connect(NULL, NULL);

    /* Get the first screen */
    xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;

    /* Create black (foreground) graphic context */
    xcb_window = screen->root;
    foreground = xcb_generate_id(connection);
    uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES ;
    uint32_t values[2] = {screen->black_pixel, 0};

    xcb_create_gc(connection, foreground, xcb_window, mask, values);

    /* Create a window */
    xcb_window = xcb_generate_id(connection);

    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    values[0] = screen->white_pixel;
    values[1] = (XCB_EVENT_MASK_EXPOSURE |
                 XCB_EVENT_MASK_BUTTON_PRESS |
                 XCB_EVENT_MASK_BUTTON_RELEASE |
                 XCB_EVENT_MASK_KEY_PRESS |
                 XCB_EVENT_MASK_POINTER_MOTION |
                 XCB_EVENT_MASK_BUTTON_MOTION);

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



}


void SV_Window::run() {
    // Map the window on the screen and flush
    xcb_map_window (connection, xcb_window);
    xcb_flush (connection);

    xcb_generic_event_t* event;
    xcb_button_press_event_t* bp;
    xcb_key_press_event_t* kp;
    timer.restart();
    while ((event = xcb_wait_for_event(connection))) {
        switch (event->response_type & ~0x80) {
            case XCB_MOTION_NOTIFY:
            case XCB_BUTTON_PRESS:
            case XCB_BUTTON_RELEASE: {
                bp = (xcb_button_press_event_t *) event;
                for (const auto &widget : widgets) {
                    if (widget->x() < bp->event_x and bp->event_x < widget->x() + widget->w() and
                        widget->y() < bp->event_y and bp->event_y < widget->y() + widget->h()) {
                        if (widget->handle(event)) {break;}
                    }
                }
                break;
            }
            case XCB_KEY_PRESS: {
                kp = (xcb_key_press_event_t *) event;
                if ((int) kp->detail == 9) {exit(0);}
                for (const auto &widget : widgets) {
                    if (widget->handle(event)) {break;}
                }
                break;
            }
            case XCB_EXPOSE: {
                break;
            }
            default:
                break;
        }
        if (timer.is_done()) {
            timer.restart();
            // If a window is queued for a redraw, add its changes to the pixels that need to be redrawn
            for (const auto& widget : widgets) {
                if (widget->needsdraw()) {
                    widget->draw();
                    /*
                     * Inserting everything into a single pixel table makes newer widgets changes overwrite older
                     * widgets changes. We actually need this behavior otherwise the drawing preference is
                     * defined by the implementation of std::sort. This is more expensive in CPU and memory, but it
                     * prevents implementation-defined behavior.
                    */
                    auto widget_changes = widget->get_changed_pixels();
                    if (not widget_changes.is_empty()) {
                        pixel_table.insert_from(widget_changes);
                        widget->clear();
                    }
                }
            }
            flush();
            xcb_flush(connection);
        }

        free(event);
    }
}


void SV_Window::add(SV_Widget* widget) {
    this->widgets.push_back(widget);
}


void SV_Window::flush() {
    if (pixel_table.is_empty() == 0) return;

    std::sort(pixel_vector.begin(), pixel_vector.end());

    std::vector<pixel> same_color_pixels;
    auto current_color = pixel_vector[0].color;
    for (const auto& px : pixel_vector) {
        if (px.color == current_color) {
            same_color_pixels.push_back(px);
        }
        else {
            xcb_point_t *points = (xcb_point_t *) malloc(same_color_pixels.size() * sizeof(xcb_point_t));
            for (int i = 0; i < same_color_pixels.size(); i++) {
                points[i] = {(int16_t) same_color_pixels[i].x, (int16_t) same_color_pixels[i].y};
            }

            xcb_change_gc(connection, foreground, XCB_GC_FOREGROUND, &current_color);
            xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, xcb_window, foreground, same_color_pixels.size(), points);

            free(points);

            current_color = px.color;
            same_color_pixels.clear();
            same_color_pixels.push_back(px);
        }
    }
    pixel_vector.clear();
}
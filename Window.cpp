#include "Window.h"
#include "Widget.h"
#include "PixelTable.h"
#include <algorithm>
#include <iostream>
#include <xcb/xcb.h>
#include <cstring>
//#include "fonts.h"
#include "Event.h"


Window::Window(int width, int height) {
    this->width = width;
    this->height = height;
    // Open the connection to the X server
    connection = xcb_connect(NULL, NULL);
    if (!connection) {
        std::cout << "unable to connect to display" << std::endl;
        exit(1);
    }

    // Get the first screen
    auto setup = xcb_get_setup(connection);
    if (!setup) {
        std::cout << "unable to connect to display" << std::endl;
        exit(1);
    }

    xcb_screen_t *screen = xcb_setup_roots_iterator(setup).data;
    if (!screen) {
        std::cout << "unable to connect to display" << std::endl;
        exit(1);
    }

    // Create black (foreground) graphic context
    xcb_window = screen->root;
    foreground = xcb_generate_id(connection);
    uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    uint32_t values[2] = {screen->black_pixel, 0};

    xcb_create_gc(connection, foreground, xcb_window, mask, values);

    // Create a window
    xcb_window = xcb_generate_id(connection);

    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    values[0] = screen->black_pixel;
    values[1] = (XCB_EVENT_MASK_EXPOSURE |
                 XCB_EVENT_MASK_BUTTON_PRESS |
                 XCB_EVENT_MASK_BUTTON_RELEASE |
                 XCB_EVENT_MASK_POINTER_MOTION |
                 XCB_EVENT_MASK_BUTTON_MOTION |
                 XCB_EVENT_MASK_KEY_PRESS |
                 XCB_EVENT_MASK_KEY_RELEASE |
                 XCB_EVENT_MASK_RESIZE_REDIRECT |
                 XCB_EVENT_MASK_FOCUS_CHANGE | XCB_EVENT_MASK_OWNER_GRAB_BUTTON
    );

    xcb_create_window(connection,                    /* connection          */
                      XCB_COPY_FROM_PARENT,          /* depth               */
                      xcb_window,                    /* window Id           */
                      screen->root,                  /* parent window       */
                      0, 0,                          /* view_x, y           */
                      width, height,                 /* width, height       */
                      0,                             /* border_width        */
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
                      screen->root_visual,           /* visual              */
                      mask, values);                 /* masks */

    const char *font_name = "-bitstream-courier 10 pitch-medium-r-normal--15-15-0-0-m-0-adobe-standard";
    xcb_font_t font = xcb_generate_id(connection);
    xcb_open_font(connection, font, strlen(font_name), font_name);

    text_gc = xcb_generate_id(connection);
    uint32_t mask2 = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT;
    uint32_t values2[3] = {screen->white_pixel, screen->black_pixel, font};
    xcb_create_gc(connection, text_gc, xcb_window, mask2, values2);

    xcb_close_font(connection, font);

    const char *title = "sanicview";
    xcb_change_property(connection,
                        XCB_PROP_MODE_REPLACE,
                        xcb_window,
                        XCB_ATOM_WM_NAME,
                        XCB_ATOM_STRING,
                        8,
                        strlen(title),
                        title);

    // Disable resizing
    xcb_size_hints_t hints;
    xcb_icccm_size_hints_set_min_size(&hints, width, height);
    xcb_icccm_size_hints_set_max_size(&hints, width, height);
    xcb_icccm_set_wm_size_hints(connection, xcb_window, XCB_ATOM_WM_NORMAL_HINTS, &hints);

    drawing_buffer = PixelTable(width, height);
    xcb_map_window(connection, xcb_window);
    xcb_flush(connection);    // Apparently this is necessary to enable drawing from widgets
}


void Window::run() {
    Widget *has_mouse = NULL;
    xcb_generic_event_t *xcb_event_ptr;
    while ((xcb_event_ptr = xcb_wait_for_event(connection))) {
        Event event(xcb_event_ptr);

        // Special case for mouse events; redirect them to any widget that has grabbed the mouse focus
        if (has_mouse && (event.type() == mouse_move || event.type() == mouse_release)) {
            has_mouse->handle(event);
            if (event.type() == mouse_release) {
                has_mouse = NULL;
            }
        } else {
            for (const auto &widget : widgets) {
                bool was_handled;
                if ((event.type() == key_press || event.type() == key_release) ||
                    (widget->x() < event.x() && event.x() < widget->x() + widget->w() &&
                     widget->y() < event.y() && event.y() < widget->y() + widget->h())) {
                    was_handled = widget->handle(event);
                    if (was_handled && event.type() == mouse_push) {
                        has_mouse = widget;
                    }
                    if (was_handled) { break; }
                }
            }
        }

        // Clean up and close if esc is pressed
        if (event.type() == key_press) {
            if ((int) ((xcb_key_press_event_t *) xcb_event_ptr)->detail == 9) {
                free(xcb_event_ptr);
                return;
            }
        }

        for (const auto &widget : widgets) {
            if (widget->needsdraw()) {
                widget->draw();
                widget->clear_draw();
            }
        }
        flush();
        xcb_flush(connection);
        free(xcb_event_ptr);
    }
}


void Window::add(Widget *widget) {
    this->widgets.push_back(widget);
}


void Window::flush() {
    if (drawing_buffer.empty()) { return; }

    auto &changed_pixels = drawing_buffer.get_changed();

    if (changed_pixels.empty()) { return; }

    auto current_color = changed_pixels.front().color;

    for (const auto &px : changed_pixels) {
        if (px.color != current_color) {
            xcb_change_gc(connection, foreground, XCB_GC_FOREGROUND, &current_color);
            xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, xcb_window, foreground, color_run.size(),
                           color_run.data());
            color_run.clear();
            current_color = px.color;
        }
        color_run.push_back({(int16_t) px.x, (int16_t) px.y});
    }

    xcb_change_gc(connection, foreground, XCB_GC_FOREGROUND, &current_color);
    xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, xcb_window, foreground, color_run.size(), color_run.data());
    color_run.clear();
    xcb_flush(connection);
}


void Window::draw_point(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    drawing_buffer.insert(x, y, r, g, b);
}


void Window::draw_point(int x, int y, uint32_t color) {
    drawing_buffer.insert(x, y, color);
}


void Window::draw_text(const std::string &text, int x, int y, uint32_t color) {
    xcb_change_gc(connection, text_gc, XCB_GC_FOREGROUND, &color);
    xcb_image_text_8(connection, text.size(), xcb_window, text_gc, x, y, text.c_str());
}


void Window::draw_line(int x1, int y1, int x2, int y2, uint32_t color) {
    xcb_change_gc(connection, foreground, XCB_GC_FOREGROUND, &color);
    xcb_segment_t line[] = {{(uint16_t) x1, (uint16_t) y1, (uint16_t) x2, (uint16_t) y2}};
    xcb_poly_segment(connection, xcb_window, foreground, 1, line);
}


void Window::draw_rectangle(int x, int y, int width, int height, uint32_t color) {
    xcb_change_gc(connection, foreground, XCB_GC_FOREGROUND, &color);
    xcb_rectangle_t rectangle[] = {{x, y, width, height}};
    xcb_poly_fill_rectangle(connection, xcb_window, foreground, 1, rectangle);
}


void Window::draw_bitmap(const Image<uint8_t> &bitmap, int x_min, int y_min) {
    for (int y = 0; y < bitmap.height(); y++) {
        for (int x = 0; x < bitmap.width(); x++) {
            if (x + x_min < width && y + y_min < height) {
                draw_point(x + x_min, y + y_min, bitmap(x, y), bitmap(x, y), bitmap(x, y));
            }
        }
    }
}


Window::~Window() {
    xcb_disconnect(connection);
}

#include "SV_Window.h"
#include "Widget.h"
#include <iostream>


SV_Window::SV_Window(int width, int height) {
    /* Open the connection to the X server */
    connection = xcb_connect(NULL, NULL);

    /* Get the first screen */
    xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;

    /* Create black (foreground) graphic context */
    xcb_window = screen->root;
    foreground = xcb_generate_id(connection);
    uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    uint32_t values[2] = {screen->black_pixel, 0};

    xcb_create_gc(connection, foreground, xcb_window, mask, values);


    /* Create a window */
    xcb_window = xcb_generate_id(connection);

    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    values[0] = screen->white_pixel;
    values[1] = XCB_EVENT_MASK_EXPOSURE;

    xcb_create_window(connection,                    /* connection          */
                      XCB_COPY_FROM_PARENT,          /* depth               */
                      xcb_window,                    /* window Id           */
                      screen->root,                  /* parent window       */
                      0, 0,                          /* x, y                */
                      width, height,                 /* width, height       */
                      10,                            /* border_width        */
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
                      screen->root_visual,           /* visual              */
                      mask, values);                      /* masks */
}


int SV_Window::run() {
    // Map the window on the screen and flush
    xcb_map_window (connection, xcb_window);
    xcb_flush (connection);

    xcb_generic_event_t *event;
    while ((event = xcb_wait_for_event (connection))) {
        switch (event->response_type & ~0x80) {
            case XCB_EXPOSE:
                for (const auto& widget : widgets) {
                    if (widget->handle(event)) break;
                }

                for (const auto& widget : widgets) {
                    if (widget->needsdraw()) widget->draw();
                }

                xcb_flush (connection);

                break;
            default:
                break;
        }

        free(event);
    }

    return 0;

}


void SV_Window::add(Widget* widget) {
    widgets.push_back(widget);
    widget->window(this);
}

void SV_Window::draw(std::vector<pixel>& all_pixels) {

    std::vector<pixel> pixels;

    

    auto black_pixels = std::vector<pixel>();

    for (const auto& px : pixels) {
        if (px.color == 0) {
            black_pixels.push_back(px);
        }
    }

    xcb_point_t* points = (xcb_point_t*)malloc(black_pixels.size()*sizeof(xcb_point_t));
    for (int i = 0; i < black_pixels.size(); i++) {
        points[i] = {(int16_t)black_pixels[i].x, (int16_t)black_pixels[i].y};
    }

    xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, xcb_window, foreground, black_pixels.size(), points);

    free(points);

    exit(0);
}

void SV_Window::draw(pixel px) {
    xcb_point_t points[] = {{(int16_t)px.x, (int16_t)px.y}};
    //uint32_t color[] = {px.color};
    //xcb_create_gc(connection, foreground, xcb_window, XCB_GC_FOREGROUND, color);
    xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, xcb_window, foreground, 4, points);
}
#include "SV_Window.h"
#include "SV_Widget.h"


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
                      mask, values);                 /* masks */
}


void SV_Window::run() {
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

                // If a window is queued for a redraw, add its changes to the pixels that need to be redrawn
                for (const auto& widget : widgets) {
                    if (widget->needsdraw()) {
                        widget->draw();
                        auto& widget_changes = widget->get_changed_pixels();
                        draw_pixels.insert(draw_pixels.end(), widget_changes.begin(), widget_changes.end());
                    }
                }

                flush();
                xcb_flush (connection);
                break;
            default:
                break;
        }
        free(event);
    }
}


void SV_Window::add(SV_Widget* widget) {
    this->widgets.push_back(widget);
}


int SV_Window::get_height() {
    return height;
}


int SV_Window::get_width() {
    return width;
}


void SV_Window::flush() {
    if (draw_pixels.size() == 0) return;

    std::sort(draw_pixels.begin(), draw_pixels.end());

    std::vector<pixel> same_color_pixels;
    auto current_color = draw_pixels[0].color;
    for (const auto& px : draw_pixels) {
        if (px.color == current_color) {
            same_color_pixels.push_back(px);
        }
        else {
            xcb_point_t *points = (xcb_point_t *) malloc(same_color_pixels.size() * sizeof(xcb_point_t));
            for (int i = 0; i < same_color_pixels.size(); i++) {
                points[i] = {(int16_t) same_color_pixels[i].x, (int16_t) same_color_pixels[i].y};
            }

            uint32_t values[1] = {(uint32_t)current_color*65793};
            xcb_change_gc(connection, foreground, XCB_GC_FOREGROUND, values);
            xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, xcb_window, foreground, same_color_pixels.size(), points);

            free(points);

            current_color = px.color;
            same_color_pixels.clear();
            same_color_pixels.push_back(px);
        }
    }
    draw_pixels.clear();
}
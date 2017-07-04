#include "CursorTracker.h"
#include "Widget.h"
#include "Window.h"


CursorTracker::CursorTracker(Window* window) : Widget(window, window->w()-200, 200, 200, 37) {
    uint32_t black = 0;
    xcb_change_gc(window->connection, window->foreground, XCB_GC_FOREGROUND, &black);
    xcb_rectangle_t rectangles[] = {
            {x(), y(), w(), h()}
    };
    xcb_poly_fill_rectangle(window->connection, window->xcb_window, window->foreground, 1, rectangles);

    buf << "Cursor x,y: " << image_x << "," << image_y;
    std::string s(std::istreambuf_iterator<char>(buf), {});
    draw_text(s, 1, 13);

    buf << "Value: " << image_value;
    std::string ss(std::istreambuf_iterator<char>(buf), {});
    draw_text(ss, 1, 31);
}


void CursorTracker::resize() {
    x(window()->w()-200);
    redraw();
}


void CursorTracker::draw() {
    int char_width = 9;

    uint32_t black = 0;
    xcb_change_gc(window()->connection, window()->foreground, XCB_GC_FOREGROUND, &black);
    xcb_rectangle_t rectangles[] = {
        {x()+char_width*12+1, y(), w(), 17},
        {x()+char_width*7+1, y()+31-13, w(), 17}
    };
    xcb_poly_fill_rectangle(window()->connection, window()->xcb_window, window()->foreground, 2, rectangles);

    buf << image_x << "," << image_y;
    std::string s(std::istreambuf_iterator<char>(buf), {});
    draw_text(s, char_width*12+1, 13);

    buf << image_value;
    std::string ss(std::istreambuf_iterator<char>(buf), {});
    draw_text(ss, char_width*7+1, 31);
}


void CursorTracker::set_location(int x, int y, double value) {
    if (x != image_x || x != image_y || value != image_value) {
        this->image_x = x;
        this->image_y = y;
        this->image_value = value;
        redraw();
    }
}

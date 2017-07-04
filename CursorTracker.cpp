#include "CursorTracker.h"
#include "Widget.h"
#include "Window.h"


CursorTracker::CursorTracker(Window* window) : Widget(window, window->w()-200, 200, 200, 37) {
    draw_rectangle(0, 0, w(), h()-1, 0);

    buf << "Cursor x,y: " << image_x << "," << image_y;
    std::string s(std::istreambuf_iterator<char>(buf), {});
    draw_text(s, 1, 13, 16777215);

    buf << "Value: " << image_value;
    std::string ss(std::istreambuf_iterator<char>(buf), {});
    draw_text(ss, 1, 31, 16777215);
}


void CursorTracker::resize() {
    x(window()->w()-200);
    redraw();
}


void CursorTracker::draw() {
    int char_width = 9;

    draw_rectangle(char_width*12+1, 1, w(), 16, 0);
    buf << image_x << "," << image_y;
    std::string s(std::istreambuf_iterator<char>(buf), {});
    draw_text(s, char_width*12+1, 13, 16777215);

    draw_rectangle(char_width*7+1, 19, w(), 16, 0);
    buf << image_value;
    std::string ss(std::istreambuf_iterator<char>(buf), {});
    draw_text(ss, char_width*7+1, 31, 16777215);
}


void CursorTracker::set_location(int x, int y, double value) {
    if (x != image_x || x != image_y || value != image_value) {
        this->image_x = x;
        this->image_y = y;
        this->image_value = value;
        redraw();
    }
}

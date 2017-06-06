#include "SV_CursorTracker.h"


SV_CursorTracker::SV_CursorTracker(SV_Window* window) : SV_Widget(window, window->w()-200, 200, 200, 45) {}


void SV_CursorTracker::resize() {
    redraw();
}


void SV_CursorTracker::draw() {
    for (int x = 100; x < w(); x++) {
        for (int y = 0; y < h(); y++) {
            draw_point(x, y, 0);
        }
    }

    buf << "Cursor x,y: " << x << "," << y;
    std::string s(std::istreambuf_iterator<char>(buf), {});
    draw_text(s, 0, 15, 12);
}


void SV_CursorTracker::set_location(int x, int y) {
    this->x = x;
    this->y = y;
    redraw();
}

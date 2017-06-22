#include "SV_CursorTracker.h"


SV_CursorTracker::SV_CursorTracker(SV_Window* window) : SV_Widget(window, window->w()-200, 200, 200, 45) {}


void SV_CursorTracker::resize() {
    x(window()->w()-200);
    redraw();
}


void SV_CursorTracker::draw() {
    for (int x = 0; x < w(); x++) {
        for (int y = 0; y < h(); y++) {
            draw_point(x, y, 0);
        }
    }

    buf << "Cursor x,y: " << image_x << "," << image_y;
    std::string s(std::istreambuf_iterator<char>(buf), {});
    draw_text(s, 0, 15, 12);

    buf << "Value: " << image_value;
    std::string ss(std::istreambuf_iterator<char>(buf), {});
    draw_text(ss, 0, 33, 12);
}


void SV_CursorTracker::set_location(int x, int y, double value) {
    if (x != image_x || x != image_y || value != image_value) {
        this->image_x = x;
        this->image_y = y;
        this->image_value = value;
        redraw();
    }
}

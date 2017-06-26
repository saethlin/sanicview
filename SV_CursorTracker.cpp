#include "SV_CursorTracker.h"


SV_CursorTracker::SV_CursorTracker(SV_Window* window) : SV_Widget(window, window->w()-200, 200, 200, 35) {}


void SV_CursorTracker::resize() {
    x(window()->w()-200);
    redraw();
}


void SV_CursorTracker::draw() {
    if (first) {
        for (int y = 0; y < h()-1; y++) {
            for (int x = 0; x < w(); x++) {
                draw_point(x, y, 0, 0, 0);
            }
        }

        buf << "Cursor x,y: " << image_x << "," << image_y;
        std::string s(std::istreambuf_iterator<char>(buf), {});
        draw_text(s, 1, 13, 12);

        buf << "Value: " << image_value;
        std::string ss(std::istreambuf_iterator<char>(buf), {});
        draw_text(ss, 1, 31, 12);
    }
    else {
        for (int y = 0; y < 16; y++) {
            for (int x = 9*12+1; x < w(); x++) {
                draw_point(x, y, 0);
            }
        }
        buf << image_x << "," << image_y;
        std::string s(std::istreambuf_iterator<char>(buf), {});
        draw_text(s, 9*12+1, 13, 12);

        for (int y = 17; y < 31; y++) {
            for (int x = 9*7+1; x < w(); x++) {
                draw_point(x, y, 0);
            }
        }
        buf << image_value;
        std::string ss(std::istreambuf_iterator<char>(buf), {});
        draw_text(ss, 9*7+1, 31, 12);
    }
    first = false;
}


void SV_CursorTracker::set_location(int x, int y, double value) {
    if (x != image_x || x != image_y || value != image_value) {
        this->image_x = x;
        this->image_y = y;
        this->image_value = value;
        redraw();
    }
}

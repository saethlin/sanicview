#include "SV_Widget.h"
#include "SV_Window.h"


SV_Widget::SV_Widget(SV_Window* window, int x, int y, int width, int height) {
    this->x(x);
    this->y(y);
    this->w(width);
    this->h(height);
    this->parent_window = window;
    window->add(this);
}


void SV_Widget::draw_point(int pixel_x, int pixel_y, unsigned char color) {
    if (pixel_x >= 0 and pixel_x < w() and
        pixel_y >= 0 and pixel_y < h()) {
        window()->draw_point(x()+pixel_x, y()+pixel_y, color, color, color);
    }
}


void SV_Widget::draw_point(int pixel_x, int pixel_y, unsigned char r, unsigned char g, unsigned char b) {
    if (pixel_x >= 0 and pixel_x < w() and
        pixel_y >= 0 and pixel_y < h()) {
        window()->draw_point(x()+pixel_x, y()+pixel_y, r, g, b);
    }
}


bool SV_Widget::needsdraw() {
    if (do_redraw) {
        do_redraw = false;
        return true;
    }
    else {
        return false;
    }
}
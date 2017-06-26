#include "SV_Widget.h"
#include "SV_Window.h"
#include "colormaps.h"


SV_Widget::SV_Widget(SV_Window *window, int x, int y, int width, int height) {
    this->x(x);
    this->y(y);
    this->w(width);
    this->h(height);
    this->parent_window = window;
    window->add(this);
}


void SV_Widget::draw_point(int pixel_x, int pixel_y, uint8_t r, uint8_t g, uint8_t b) {
    window()->draw_point(x() + pixel_x, y() + pixel_y, r, g, b);
}


void SV_Widget::draw_text(const std::string& text, int x, int y, int pt) {
    window()->draw_text(text, x + x_impl, y + y_impl, pt);
}


void SV_Widget::draw_point(int pixel_x, int pixel_y, uint8_t color) {
    window()->draw_point(x() + pixel_x, y() + pixel_y, inferno[color]);
}

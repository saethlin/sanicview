#include "SV_Widget.h"
#include "SV_Window.h"
#include "colormaps.h"


SV_Widget::SV_Widget(SV_Window *window, int x, int y, int width, int height) {
    this->parent_window = window;
    window->add(this);
    this->x(x);
    this->y(y);
    this->w(width);
    this->h(height);
}


void SV_Widget::draw_point(int pixel_x, int pixel_y, uint8_t r, uint8_t g, uint8_t b) {
    window()->draw_point(x() + pixel_x, y() + pixel_y, r, g, b);
}


void SV_Widget::draw_text(const std::string& text, int text_x, int text_y, int pt) {
    window()->draw_text(text, x()+text_x, y()+text_y, pt);
}


void SV_Widget::draw_point(int pixel_x, int pixel_y, uint8_t color) {
    window()->draw_point(x() + pixel_x, y() + pixel_y, inferno[color]);
}


void SV_Widget::x(int x) {
    x_impl = x;
    if (x != 0) x_impl += border;
}


void SV_Widget::y(int y) {
    y_impl = y;
    if (y_impl != 0) y_impl += border;
}


void SV_Widget::h(int height) {
    this->height = height;
    if (height != window()->h()) this->height -= border;
}


void SV_Widget::w(int width) {
    this->width = width;
    if (this->width != window()->w()) this->width -= border;
}

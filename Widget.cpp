#include "Widget.h"
#include "Window.h"
#include "colormaps.h"


Widget::Widget(Window* window, int x, int y, int width, int height) {
    this->parent_window = window;
    window->add(this);
    this->x(x);
    this->y(y);
    this->w(width);
    this->h(height);
    for (int x_border = x; x_border < x+width; x_border++) {
        if (y != 0) window->draw_point(x_border, y, border_color); // Top
        if (y+height != window->h()) window->draw_point(x_border, y+height-1, border_color); // Bottom
    }
    for (int y_border = y; y_border < y+height; y_border++) {
        if (x != 0) window->draw_point(x, y_border, border_color); // Left
        if (x+width != window->w()) window->draw_point(x+width-1, y_border, border_color); // Right
    }
}


void Widget::draw_point(int pixel_x, int pixel_y, uint8_t r, uint8_t g, uint8_t b) {
    window()->draw_point(x() + pixel_x, y() + pixel_y, r, g, b);
}


void Widget::draw_text(const std::string& text, int text_x, int text_y) {
    window()->draw_text(text, x()+text_x, y()+text_y);
}


void Widget::draw_point(int pixel_x, int pixel_y, uint8_t color) {
    window()->draw_point(x() + pixel_x, y() + pixel_y, inferno[color]);
}


void Widget::x(int x) {
    x_impl = x;
    if (x != 0) x_impl += border;
}


void Widget::y(int y) {
    y_impl = y;
    if (y_impl != 0) y_impl += border;
}


void Widget::h(int height) {
    this->height = height;
    if (height != window()->h()) this->height -= border;
}


void Widget::w(int width) {
    this->width = width;
    if (this->width != window()->w()) this->width -= border;
}

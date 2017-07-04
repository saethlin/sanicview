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

    if (y != 0) window->draw_line(x, y, x+width-1, y, border_color); // Top
    if (y+height != window->h()) window->draw_line(x, y+height-1, x+width-1, y+height-1, border_color); // Bottom
    if (x != 0) window->draw_line(x, y, x, y+height-1, border_color); // Left
    if (x+width != window->w()) window->draw_line(x+width-1, y, x+width-1, y+height-1, border_color); // Right
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


void Widget::draw_line(int x1, int y1, int x2, int y2, uint32_t color) {
    window()->draw_line(x()+x1, y()+y1, x()+x2, y()+y2, color);
}


void Widget::draw_rectangle(int x1, int y1, int x2, int y2, uint32_t color) {
    window()->draw_rectangle(x()+x1, y()+y1, x()+x2, y()+y2, color);
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

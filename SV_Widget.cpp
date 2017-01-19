#include "SV_Widget.h"
#include "SV_Window.h"


void SV_Widget::change_pixel(int pixel_x, int pixel_y, unsigned char value) {
    changed_pixels.push_back({pixel_x, pixel_y, value});
}


std::vector<pixel>& SV_Widget::get_changed_pixels() {
    return changed_pixels;
}


void SV_Widget::clear() {
    changed_pixels.clear();
    do_redraw = false;
}


bool SV_Widget::needsdraw() {
    return do_redraw;
}


void SV_Widget::redraw() {
    do_redraw = true;
}


SV_Window* SV_Widget::window() {
    return this->parent_window;
}

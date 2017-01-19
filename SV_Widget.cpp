#include "SV_Widget.h"
#include "SV_Window.h"


SV_Widget::SV_Widget(SV_Window* window) {
    this->parent_window = window;
    window->add(this);
}


void SV_Widget::change_pixel(int pixel_x, int pixel_y, const unsigned char color) {
    changed_pixels.push_back({pixel_x, pixel_y, (uint32_t)color*65793});
}


void SV_Widget::change_pixel(int pixel_x, int pixel_y, const unsigned char color[3]) {
    auto value = (uint32_t)color[0]*65536+(uint32_t)color[1]*256+(uint32_t)color[2];
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


bool SV_Widget::handle(xcb_generic_event_t* event) {
    return false;
}
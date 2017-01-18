#include "Widget.h"
#include "SV_Window.h"
#include <iostream>

Widget::Widget(int x, int y, CImg<double>& image) {
    this->x = x;
    this->y = y;
    this->image = image;
}


void Widget::draw() {
    std::vector<double> sortable(image.data(), image.data() + image.size());
    auto median = sortable.begin() + (sortable.size()/2);
    auto white_level = sortable.begin() + sortable.size()*0.997;
    std::nth_element(sortable.begin(), median, sortable.end());
    std::nth_element(sortable.begin(), white_level, sortable.end());
    double black = *median;
    double white = *white_level;

    CImg<unsigned char> clipped = (image.get_cut(black, white) - black).normalize(0, 255);

    auto cropped = clipped.get_crop(x, y, x+800, y+500);

    std::vector<pixel> pixels;
    for (auto y = 0; y < cropped.height(); y++) {
        for (auto x = 0; x < cropped.width(); x++) {
            pixels.push_back({x, y, cropped(x, y)});
        }
    }

    window()->draw(pixels);
}


bool Widget::needsdraw(){
    return redraw;
}


int Widget::handle(xcb_generic_event_t* event) {
    return 0;
}


SV_Window* Widget::window() {
    return this->parent_window;
}


void Widget::window(SV_Window* window) {
    this->parent_window = window;
}
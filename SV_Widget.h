#ifndef SANICVIEW_WIDGET_H
#define SANICVIEW_WIDGET_H

#include <vector>
#include <xcb/xcb.h>
#include "pixel.h"

#include <CImg.h>
using namespace cimg_library;

class SV_Window;

class SV_Widget {
public:
    SV_Widget(SV_Window* window, int x0, int y0, int width, int height);
    virtual void draw() {};
    bool needsdraw();
    SV_Window* window();
    void redraw();
    std::vector<pixel>& get_changed_pixels();
    void change_pixel(int pixel_x, int pixel_y, const unsigned char value);
    void change_pixel(int pixel_x, int pixel_y, const unsigned char value[3]);
    void clear();
    virtual bool handle(xcb_generic_event_t* event);
    virtual void resize() {};
    inline int x() {return x0;};
    inline int y() {return y0;};
    inline int w() {return width;};
    inline int h() {return height;};
    inline void set_x(int x) {x0 = x;};
    inline void set_y(int y) {y0 = y;};
    inline void set_width(int width) {this->width = width;};
    inline void set_height(int height) {this->height = height;};

private:
    SV_Window* parent_window;
    int x0, y0, width, height;
    bool do_redraw = true;
    CImg<double> image;
    std::vector<pixel> changed_pixels;
    double black, white;
};


#endif //SANICVIEW_WIDGET_H

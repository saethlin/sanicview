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
    SV_Widget(SV_Window*);
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

private:
    SV_Window* parent_window;
    int x, y, width, height;
    bool do_redraw = true;
    CImg<double> image;
    std::vector<pixel> changed_pixels;
    double black, white;
};


#endif //SANICVIEW_WIDGET_H

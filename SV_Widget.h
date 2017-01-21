#ifndef SANICVIEW_WIDGET_H
#define SANICVIEW_WIDGET_H

#include <unordered_set>
#include <xcb/xcb.h>
#include "pixel.h"
#include "SV_PixelTable.h"
#include "SV_Event.h"

#include <CImg.h>
using namespace cimg_library;

class SV_Window;

class SV_Widget {
public:
    SV_Widget(SV_Window* window, int x0, int y0, int width, int height);
    void draw() {};
    bool needsdraw();
    SV_Window* window();
    void redraw();
    SV_PixelTable& get_changed_pixels();
    void change_pixel(int pixel_x, int pixel_y, const unsigned char value);
    void change_pixel(int pixel_x, int pixel_y, const unsigned char value[3]);
    void clear();
    bool handle(SV_Event event);
    void resize() {};
    inline int x() {return x0;};
    inline void x(int x) {x0 = x;};
    inline int y() {return y0;};
    inline void y(int y) {y0 = y;};
    inline int w() {return width;};
    inline void w(int width) {this->width = width;};
    inline int h() {return height;};
    inline void h(int height) {this->height = height;};

private:
    SV_Window* parent_window;
    int x0, y0, width, height;
    bool do_redraw = true;
    CImg<double> image;
    SV_PixelTable changed_pixels;
    double black, white;
};


#endif //SANICVIEW_WIDGET_H

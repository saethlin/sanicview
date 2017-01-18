#ifndef SANICVIEW_WIDGET_H
#define SANICVIEW_WIDGET_H

#include <vector>
#include <xcb/xcb.h>
#include "pixel.h"

#include <CImg.h>
using namespace cimg_library;

class SV_Window;

class Widget {
public:
    Widget(int x, int y, CImg<double>& image);
    void draw();
    bool needsdraw();
    SV_Window* window();
    void window(SV_Window* window);
    int handle(xcb_generic_event_t* event);
    void draw_points(std::vector<pixel>);
private:
    SV_Window* parent_window;
    int x, y, width, height;
    bool redraw = true;
    CImg<double> image;
};


#endif //SANICVIEW_WIDGET_H

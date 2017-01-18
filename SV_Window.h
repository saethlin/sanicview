#ifndef SANICVIEW_WINDOW_H
#define SANICVIEW_WINDOW_H

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <xcb/xcb.h>
#include "pixel.h"

#include <CImg.h>
using namespace cimg_library;

class Widget;

class SV_Window {
public:
    SV_Window(int width, int height);
    int run();
    void add(Widget*);
    void draw(pixel);
    void draw(std::vector<pixel>&);
    void draw(CImg<unsigned char>&);
private:
    xcb_connection_t *connection;
    xcb_drawable_t xcb_window;
    xcb_gcontext_t foreground;
    std::vector<Widget*> widgets;
};


#endif //SANICVIEW_WINDOW_H

#ifndef SANICVIEW_WINDOW_H
#define SANICVIEW_WINDOW_H

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <xcb/xcb.h>
#include "pixel.h"

#include <CImg.h>
using namespace cimg_library;

class SV_Widget;

class SV_Window {
public:
    SV_Window(int width, int height);
    int run();
    void add(SV_Widget*);
    void draw(const pixel&);
    void draw(const std::vector<pixel>&);
    void draw(CImg<unsigned char>&);
    void flush();
    int get_width();
    int get_height();
private:
    int width, height;
    xcb_connection_t *connection;
    xcb_drawable_t xcb_window;
    xcb_gcontext_t foreground;
    xcb_alloc_color_reply_t* color_table[255];
    std::vector<SV_Widget*> widgets;
    std::vector<pixel> draw_pixels;
};


#endif //SANICVIEW_WINDOW_H

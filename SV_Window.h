#ifndef SANICVIEW_WINDOW_H
#define SANICVIEW_WINDOW_H

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <xcb/xcb.h>
#include "pixel.h"
#include "SV_Timer.h"

class SV_Widget;

class SV_Window {
public:
    SV_Window(int width, int height);
    void add(SV_Widget*);
    void run();
    inline int w() {return width;};
    inline int h() {return height;};
private:
    void flush();
    SV_Timer timer = SV_Timer(std::chrono::milliseconds(166));
    int width, height;
    xcb_connection_t *connection;
    xcb_drawable_t xcb_window;
    xcb_gcontext_t foreground;
    std::vector<SV_Widget*> widgets;
    std::vector<pixel> draw_pixels;
};


#endif //SANICVIEW_WINDOW_H

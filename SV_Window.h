#ifndef SANICVIEW_WINDOW_H
#define SANICVIEW_WINDOW_H

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <xcb/xcb.h>
#include "SV_Timer.h"
#include "SV_PixelTable.h"

class SV_Widget;

class SV_Window {
public:
    SV_Window(int width, int height);
    ~SV_Window();
    void add(SV_Widget*);
    void run();
    inline int w() {return width;};
    inline int h() {return height;};
    void draw_point(int x, int y, unsigned char r, unsigned char g, unsigned char b);
private:
    void flush();
    SV_Timer<std::chrono::milliseconds> timer = SV_Timer<std::chrono::milliseconds>(std::chrono::milliseconds(16));
    int width, height;
    xcb_connection_t* connection;
    xcb_drawable_t xcb_window;
    xcb_gcontext_t foreground;
    std::vector<SV_Widget*> widgets;
    SV_PixelTable drawing_buffer;
    std::vector<xcb_point_t> color_run;
};


#endif //SANICVIEW_WINDOW_H

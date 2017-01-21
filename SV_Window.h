#ifndef SANICVIEW_WINDOW_H
#define SANICVIEW_WINDOW_H

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <xcb/xcb.h>
#include "pixel.h"
#include "SV_Timer.h"
#include "SV_PixelTable.h"

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
    SV_Timer<std::chrono::milliseconds> timer = SV_Timer<std::chrono::milliseconds>(std::chrono::milliseconds(166));
    int width, height;
    xcb_connection_t *connection;
    xcb_drawable_t xcb_window;
    xcb_gcontext_t foreground;
    std::vector<SV_Widget*> widgets;
    SV_PixelTable pixel_table;
    std::vector<pixel> pixel_vector;
};


#endif //SANICVIEW_WINDOW_H

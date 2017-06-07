#ifndef SANICVIEW_WINDOW_H
#define SANICVIEW_WINDOW_H

#include "SV_PixelTable.h"
#include <xcb/xcb_icccm.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>
#include <ratio>
#include <algorithm>


class SV_Widget;

class SV_Window {
public:
    SV_Window(int width, int height, int framerate);
    ~SV_Window();
    void add(SV_Widget*);
    void run();
    int w() const {return width;}
    int h() const {return height;}
    void w(int width) {this->width = width;}
    void h(int height) {this->height = height;}
    void draw_point(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    void draw_loop();
private:
    void flush();
    bool is_first_draw = true;
    int width, height;
    xcb_connection_t* connection;
    xcb_drawable_t xcb_window;
    xcb_gcontext_t foreground;
    std::vector<SV_Widget*> widgets;
    SV_PixelTable drawing_buffer;
    std::vector<xcb_point_t> color_run;
    std::mutex lock;
    std::atomic_bool thread_alive;
    std::chrono::duration<float, std::milli> framerate;
};


#endif //SANICVIEW_WINDOW_H

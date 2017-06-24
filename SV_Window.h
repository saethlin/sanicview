#ifndef SANICVIEW_WINDOW_H
#define SANICVIEW_WINDOW_H

#include "SV_PixelTable.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>
#include <ratio>
#include <algorithm>
#include <xcb/xcb_icccm.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H


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
    void draw_point(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    void draw_point(int x, int y, uint8_t color);
    void draw_text(std::string text, int x, int y, int pt);
    void draw_bitmap(const FT_Bitmap& bitmap, FT_Int x_min, FT_Int y_min);
    void draw_loop();
private:
    void flush();
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
    FT_Library library;
    FT_Face face;
};


#endif

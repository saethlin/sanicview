#ifndef SANICVIEW_WINDOW_H
#define SANICVIEW_WINDOW_H

#include "PixelTable.h"
#include "Image.h"
#include "fonts.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>
#include <xcb/xcb_icccm.h>

class Widget;

class Window {
public:
    Window(int width, int height);
    ~Window();
    void add(Widget*);
    void run();
    int w() const {return width;}
    int h() const {return height;}
    void w(int width) {this->width = width;}
    void h(int height) {this->height = height;}
    void draw_point(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    void draw_point(int x, int y, uint32_t color);
    void draw_text(std::string text, int x, int y);
    void draw_bitmap(const Image<uint8_t>& bitmap, int x_min, int y_min);
private:
    void flush();
    int width, height;
    xcb_connection_t* connection;
    xcb_drawable_t xcb_window;
    xcb_gcontext_t foreground;
    std::vector<Widget*> widgets;
    PixelTable drawing_buffer;
    std::vector<xcb_point_t> color_run;
    std::array<glyph, 256> glyphs = ubuntu_mono_bold;
};


#endif

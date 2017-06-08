#ifndef SANICVIEW_WIDGET_H
#define SANICVIEW_WIDGET_H

#include "SV_PixelTable.h"
#include "SV_Event.h"
#include <string>
#include <xcb/xcb.h>

class SV_Window;

class SV_Widget {
public:
    SV_Widget(SV_Window* window, int x0, int y0, int width, int height);
    virtual void draw() {};
    virtual bool handle(const SV_Event& event) {return false;}
    virtual void resize() {};
    void draw_point(int pixel_x, int pixel_y, unsigned char value);
    void draw_point(int pixel_x, int pixel_y, unsigned char r, unsigned char g, unsigned char b);
    void draw_text(std::string text, int x, int y, int pt);
    SV_Window* window() const {return parent_window;}
    int x() const {return x_impl;}
    void x(int x) {x_impl = x;}
    int y() const {return y_impl;}
    void y(int y) {y_impl = y;}
    int w() const {return width;}
    void w(int width) {this->width = width;}
    int h() const {return height;}
    void h(int height) {this->height = height;}
    void redraw() {do_redraw = true;}
    bool needsdraw() {return do_redraw;}
    void clear_draw() {do_redraw = false;}

private:
    SV_Window* parent_window;
    int x_impl, y_impl, width, height;
    bool do_redraw = true;
};


#endif

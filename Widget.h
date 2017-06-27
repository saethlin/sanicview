#ifndef SANICVIEW_WIDGET_H
#define SANICVIEW_WIDGET_H

#include <string>
#include <cstdint>

class Window;
class Event;

class Widget {
public:
    Widget(Window* window, int x0, int y0, int width, int height);
    virtual void draw() {};
    virtual bool handle(const Event& event) {return false;}
    virtual void resize() {};
    void draw_point(int pixel_x, int pixel_y, uint8_t value);
    void draw_point(int pixel_x, int pixel_y, uint8_t r, uint8_t g, uint8_t b);
    void draw_text(const std::string& text, int x, int y);
    Window* window() const {return parent_window;}
    int x() const {return x_impl;}
    int y() const {return y_impl;}
    int w() const {return width;}
    int h() const {return height;}
    void x(int x);
    void y(int y);
    void w(int width);
    void h(int height);
    void redraw() {do_redraw = true;}
    bool needsdraw() {return do_redraw;}
    void clear_draw() {do_redraw = false;}

private:
    Window* parent_window = NULL;
    int x_impl = 0, y_impl = 0, width = 0, height = 0;
    bool do_redraw = false;
    int border = 1;
    uint32_t border_color = (uint32_t)((125 << 16) | (125 << 8) | 125);
};

#endif

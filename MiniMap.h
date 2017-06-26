#ifndef SANICVIEW_SV_MINIMAP_H
#define SANICVIEW_SV_MINIMAP_H

#include "Window.h"
#include "Widget.h"
#include "Event.h"
#include "Image.h"

class Display;
class Histogram;
class SV_DirList;
class CursorTracker;

class MiniMap : public Widget {
public:
    MiniMap(Window* window);
    void set_imagedisplay(Display* imagedisplay);
    void draw() override;
    bool handle(const Event& event) override;
    void resize() override;
    void set_image(Image<float>& image);
    void set_black(double black);
    void set_white(double white);
    void set_width(int width);
    void set_height(int height);
    void set_origin(int x, int y);
    void set_zoom(double zoom);

private:
    Image<float> image;
    Image<uint8_t> clipped;
    double black, white;
    bool clicked = false;
    int view_x = 0, view_y = 0, last_x = 0, last_y = 0;
    int x0_border = 0, x1_border = 0, y0_border = 0, y1_border = 0;
    bool clip, move, do_resize;
    int original_width, original_height;
    Display* imagedisplay;
};


#endif

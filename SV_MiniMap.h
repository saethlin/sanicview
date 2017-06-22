#ifndef SANICVIEW_SV_MINIMAP_H
#define SANICVIEW_SV_MINIMAP_H

#include "SV_Window.h"
#include "SV_Widget.h"
#include "SV_Event.h"
#include "SV_Image.h"

class SV_Display;
class SV_Histogram;
class SV_DirList;
class SV_CursorTracker;

class SV_MiniMap : public SV_Widget {
public:
    SV_MiniMap(SV_Window* window);
    void set_imagedisplay(SV_Display* imagedisplay);
    void draw() override;
    bool handle(const SV_Event& event) override;
    void resize() override;
    void set_image(SV_Image<double>& image);
    void set_black(double black);
    void set_white(double white);
    void set_width(int width);
    void set_height(int height);
    void set_origin(int x, int y);
    void set_zoom(double zoom);

private:
    SV_Image<double> image;
    SV_Image<unsigned char> clipped;
    double black, white;
    bool clicked = false;
    int view_x = 0, view_y = 0, last_x = 0, last_y = 0;
    int x0_border = 0, x1_border = 0, y0_border = 0, y1_border = 0;
    bool clip, move, do_resize;
    int original_width, original_height;
    SV_Display* imagedisplay;
};


#endif //SANICVIEW_SV_MINIMAP_H

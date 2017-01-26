//
// Created by ben on 1/25/17.
//

#ifndef SANICVIEW_SV_MINIMAP_H
#define SANICVIEW_SV_MINIMAP_H

#include "SV_Window.h"
#include "SV_Widget.h"
#include "SV_Event.h"

#include <CImg.h>
using namespace cimg_library;

class SV_Image;
class SV_Histogram;
class SV_DirList;
class SV_CursorTracker;

class SV_MiniMap : public SV_Widget {
public:
    SV_MiniMap(SV_Window* window);
    void set_imagedisplay(SV_Image* imagedisplay);
    void draw() override;
    void set_image(CImg<double>& image);
    void set_black(double black);
    void set_white(double white);
    void set_width(int width);
    void set_height(int height);
    void set_origin(int x, int y);
    void set_zoom(double zoom);
    bool handle(SV_Event event) override;

private:
    CImg<double> image;
    CImg<unsigned char> clipped;
    double black, white;
    int x = 0, y = 0, last_x = 0, last_y = 0;
    int x0_border = 0, x1_border = 0, y0_border = 0, y1_border = 0;
    bool clip, move;
    int original_width, original_height;
    std::vector<pixel> damaged_pixels;
    SV_Image* imagedisplay;

};


#endif //SANICVIEW_SV_MINIMAP_H

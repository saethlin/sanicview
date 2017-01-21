#ifndef SANICVIEW_SV_HISTOGRAM_H
#define SANICVIEW_SV_HISTOGRAM_H

#include "SV_Window.h"
#include "SV_Widget.h"

class SV_Image;

class SV_Histogram : public SV_Widget {
public:
    SV_Histogram(SV_Window* window);
    void draw();
    bool handle(SV_Event event);
    void set_image(CImg<double>& image);
    void set_imagedisplay(SV_Image* imagedisplay);
private:
    CImg<unsigned char> histogram;
    CImg<unsigned char> scaled;
    std::vector<double> histogram_to_value;
    SV_Image* imagedisplay;
    double black_slider, white_slider;
    int black_pos, white_pos;
    int new_black_pos, new_white_pos;
    int clicked;
    CImg<unsigned char> black_column, white_column;
};


#endif //SANICVIEW_SV_HISTOGRAM_H

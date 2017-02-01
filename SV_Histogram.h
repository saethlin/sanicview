#ifndef SANICVIEW_SV_HISTOGRAM_H
#define SANICVIEW_SV_HISTOGRAM_H

#include "SV_Widget.h"
#include "SV_Event.h"
#include "SV_Image.h"
#include <algorithm>


enum ClickState {BLACK, WHITE, NONE};

class SV_Window;
class SV_Display;

class SV_Histogram : public SV_Widget {
public:
    SV_Histogram(SV_Window* window);
    void draw() override;
    bool handle(const SV_Event& event) override;
    void set_image(SV_Image<double>& image);
    void set_imagedisplay(SV_Display* imagedisplay);
private:
    SV_Image<unsigned char> histogram;
    SV_Image<unsigned char> scaled;
    std::vector<double> histogram_to_value;
    SV_Display* imagedisplay;
    double black_slider, white_slider;
    int black_pos, white_pos;
    int new_black_pos, new_white_pos;
    int clicked = NONE;
};


#endif //SANICVIEW_SV_HISTOGRAM_H

#ifndef SANICVIEW_SV_HISTOGRAM_H
#define SANICVIEW_SV_HISTOGRAM_H

#include "SV_Widget.h"
#include "SV_Event.h"
#include "SV_Image.h"
#include <algorithm>
#include <vector>
#include <cmath>


enum ClickState {BLACK, WHITE, NONE};

class SV_Window;
class SV_Display;

class SV_Histogram : public SV_Widget {
public:
    SV_Histogram(SV_Window* window);
    void draw() override;
    bool handle(const SV_Event& event) override;
    void resize() override;
    void set_image(SV_Image<float>& image);
    void set_imagedisplay(SV_Display* imagedisplay);
private:
    SV_Image<uint8_t> histogram;
    SV_Image<uint8_t> scaled;
    std::vector<float> histogram_to_value;
    SV_Display* imagedisplay;
    double black_level, white_level;
    int black_pos, white_pos;
    int last_black = 0, last_white = 0;
    int clicked = NONE;
};


#endif

#ifndef SANICVIEW_SV_HISTOGRAM_H
#define SANICVIEW_SV_HISTOGRAM_H

#include "Widget.h"
#include "Image.h"
#include <vector>
#include <stdint.h>

enum ClickState {BLACK, WHITE, NONE};

class Window;
class Display;
class Event;

class Histogram : public Widget {
public:
    Histogram(Window* window);
    void draw() override;
    bool handle(const Event& event) override;
    void resize() override;
    void set_image(const Image<float>& image);
    void set_imagedisplay(Display* imagedisplay);
private:
    std::vector<float> histogram_to_value;
    Display* imagedisplay;
    double black_level, white_level;
    int black_pos, white_pos;
    int last_black = 0, last_white = 0;
    std::vector<uint16_t> data;
    int clicked = NONE;
};

#endif

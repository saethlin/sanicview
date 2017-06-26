#ifndef SANICVIEW_SV_CURSORTRACKER_H
#define SANICVIEW_SV_CURSORTRACKER_H

#include <string>
#include <sstream>
#include "SV_Window.h"
#include "SV_Widget.h"

class SV_CursorTracker : public SV_Widget {
public:
    SV_CursorTracker(SV_Window* window);
    void draw() override;
    void resize() override;
    void set_location(int x, int y, double value);

private:
    int image_x = 0, image_y = 0;
    double image_value = 0.0;
    bool first = true;
    std::stringstream buf;
};


#endif

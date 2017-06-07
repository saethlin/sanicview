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
    int x, y;
    double value;
    std::stringstream buf;
    std::string display_string;
};


#endif

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
    void set_location(int x, int y);

private:
    int x, y;
    std::stringstream buf;
    std::string display_string;
};


#endif

#ifndef SANICVIEW_PIXEL_H
#define SANICVIEW_PIXEL_H

#include <xcb/xcb.h>

struct pixel {
    int x = -1;
    int y = -1;
    uint32_t color;
    bool operator<(const pixel& other) const {return color < other.color;}
    void operator=(const pixel& other) {
        x = other.x;
        y = other.y;
        color = other.color;
    }
};


#endif //SANICVIEW_PIXEL_H

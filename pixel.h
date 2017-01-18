//
// Created by ben on 1/18/17.
//

#ifndef SANICVIEW_PIXEL_H
#define SANICVIEW_PIXEL_H

struct pixel {
    int x, y;
    unsigned char color;
    bool operator<(const pixel& other) const {
        return color < other.color;
    }
};

#endif //SANICVIEW_PIXEL_H

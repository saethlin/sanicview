#ifndef SANICVIEW_PIXEL_H
#define SANICVIEW_PIXEL_H

struct pixel {
    int x, y;
    uint32_t color;
    bool operator<(const pixel& other) const {
        return color < other.color;
    }
};

#endif //SANICVIEW_PIXEL_H

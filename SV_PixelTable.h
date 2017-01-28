#ifndef SANICVIEW_SV_PIXELTABLE_H
#define SANICVIEW_SV_PIXELTABLE_H

#include <vector>
#include <cstdint>

struct pixel {
    unsigned char red = 0;
    unsigned char green = 0;
    unsigned char blue = 0;
    unsigned char alpha = 0;
};


struct xcb_pixel {
    int x;
    int y;
    uint32_t color;
    const bool operator<(const xcb_pixel& other) const {return color < other.color;}
};


class SV_PixelTable {
public:
    SV_PixelTable() {}
    SV_PixelTable(int x_max, int y_max);
    void insert(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    bool empty();
    std::vector<xcb_pixel>& get_changed();

private:
    std::vector<pixel> table;
    std::vector<int> changed_inds;
    std::vector<xcb_pixel> changed;
    std::vector<std::vector<int> > changed_by_color = std::vector<std::vector<int> >(256, std::vector<int>(256, 0));
    int x_max = 0;
};


#endif //SANICVIEW_SV_PIXELTABLE_H

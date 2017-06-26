#ifndef SANICVIEW_SV_PIXELTABLE_H
#define SANICVIEW_SV_PIXELTABLE_H

#include <vector>
#include <algorithm>
#include <cstdint>

struct xcb_pixel {
    int x;
    int y;
    uint32_t color;
    const bool operator<(const xcb_pixel& other) const {return color < other.color;}
};


class PixelTable {
public:
    PixelTable() {}
    PixelTable(int x_max, int y_max);
    void insert(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    void insert(int x, int y, uint32_t color);
    void downsize(int width, int height);
    bool empty() const;
    std::vector<xcb_pixel>& get_changed();

private:
    std::vector<bool> damaged_rows;
    std::vector<uint32_t> current;
    std::vector<uint32_t> changes;
    std::vector<xcb_pixel> changed;
    int x_max = 0;
    bool empty_impl = true;
};


#endif

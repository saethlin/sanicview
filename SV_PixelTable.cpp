#include "SV_PixelTable.h"


SV_PixelTable::SV_PixelTable(int x_max, int y_max) {
    current = std::vector<uint32_t>(x_max*y_max, 0);
    changes = std::vector<uint32_t>(x_max*y_max, 0);
    this->x_max = x_max;
}


void SV_PixelTable::insert(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    empty_impl = false;
    changes[y*x_max + x] = (uint32_t)((r << 16) | (g << 8) | b);
}


std::vector<xcb_pixel>& SV_PixelTable::get_changed() {
    changed.clear();

    int y_max = changes.size()/x_max;
    for (int y = 0; y < y_max; y++) {
        for (int x = 0; x < x_max; x++) {
            int i = y*x_max + x;
            if (changes[i] != current[i]) {
                current[i] = changes[i];
                changed.push_back({x, y, changes[i]});
            }
        }
    }

    std::sort(changed.begin(), changed.end());
    empty_impl = false;
    return changed;
}


void SV_PixelTable::downsize(int width, int height) {
    int y_max = changes.size()/x_max;
    for (int y = 0; y < y_max; y++) {
        for (int x = width; x < x_max; x++) {
            current[y*x_max + x] = 0;
        }
    }

    for (int y = height; y < y_max; y++) {
        for (int x = 0; x < x_max; x++) {
            current[y*x_max + x] = 0;
        }
    }
}


bool SV_PixelTable::empty() const {
    return empty_impl;
}

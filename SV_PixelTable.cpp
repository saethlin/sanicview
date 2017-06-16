#include "SV_PixelTable.h"


SV_PixelTable::SV_PixelTable(int x_max, int y_max) {
    current = std::vector<uint32_t>(x_max*y_max);
    changes = std::vector<uint32_t>(x_max*y_max);
    this->x_max = x_max;
}


void SV_PixelTable::insert(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    changes[y*x_max + x] = (uint32_t)((r << 24) | (g << 16) | (b << 8));
 }


std::vector<xcb_pixel>& SV_PixelTable::get_changed() {
    changed.clear();

    int y_max = changes.size()/x_max;
    for (int y = 0; y < y_max; y++) {
        for (int x = 0; x < x_max; x++) {
            int i = y*x_max + y;
            if (changes[i] != current[i]) {
                current[i] = changes[i];
                changed.push_back({x, y, changes[i]});
            }
        }
    }

    std::sort(changes.begin(), changes.end());

    return changed;
}


bool SV_PixelTable::empty() const {return false;}

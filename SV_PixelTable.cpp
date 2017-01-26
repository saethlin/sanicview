#include "SV_PixelTable.h"
#include <iostream>


SV_PixelTable::SV_PixelTable(int x_max, int y_max) {
    table = std::vector<pixel>(x_max*y_max, {0, 0, 0, 0});
    this->x_max = x_max;
}


void SV_PixelTable::insert(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    auto& px = table[y*x_max + x];
    if (px.red != r or px.green != g or px.blue != b) {
        if (px.alpha == 0) {changed_inds.push_back(y*x_max + x);}
        px = {r, g, b, 255};
        empty_impl = false;
    }
}


std::vector<xcb_pixel> SV_PixelTable::get_changed() {
    std::vector<xcb_pixel> changed;
    for (auto& ind : changed_inds) {
        changed.push_back({ind%x_max, ind/x_max, (uint32_t)65536*table[ind].red + (uint32_t)256*table[ind].green + (uint32_t)table[ind].blue});
    }
    return changed;
}


void SV_PixelTable::clear() {
    for (auto& ind : changed_inds) {table[ind].alpha = 0;}
    changed_inds.clear();
    empty_impl = true;
}


bool SV_PixelTable::empty() {return empty_impl;}

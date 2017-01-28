#include "SV_PixelTable.h"


SV_PixelTable::SV_PixelTable(int x_max, int y_max) {
    table = std::vector<pixel>(x_max*y_max, {0, 0, 0, 0});
    this->x_max = x_max;
}


void SV_PixelTable::insert(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    auto& px = table[y*x_max + x];
    if (px.red != r or px.green != g or px.blue != b) {
        if (px.alpha == 0) {changed_inds.push_back(y*x_max + x);}
        px = {r, g, b, 255};
    }
}


std::vector<xcb_pixel>& SV_PixelTable::get_changed() {
    changed.clear();
    changed.reserve(changed_inds.size());

    for (auto& ind : changed_inds) {
        changed_by_color[table[ind].red].push_back(ind);
        table[ind].alpha = 0;
    }
    changed_inds.clear();

    for (auto& section : changed_by_color) {
        for (const auto& ind : section) {
            changed.push_back({ind%x_max, ind/x_max, (uint32_t)65536*table[ind].red + (uint32_t)256*table[ind].green + (uint32_t)table[ind].blue});
        }
        section.resize(table.size()/256);
        section.clear();
    }

    return changed;
}


bool SV_PixelTable::empty() {return changed_inds.empty();}

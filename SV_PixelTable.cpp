#include "SV_PixelTable.h"
#include <iostream>


SV_PixelTable::SV_PixelTable(int max_val1, int max_val2){
    table = std::vector<pixel>(hash(max_val1, max_val2));
    std::cout << table[0].x << std::endl;
}


void SV_PixelTable::insert(pixel px) {
    auto index = hash(px.x, px.y);
    if (index < table.size())  {
        table[index] = px;
    }
    else {
        table.resize(index+1);
        table[index] = px;
    }
    empty = false;
}


void SV_PixelTable::clear() {
    for (auto& px : table) {
        px.x = -1;
    }
    empty = true;
}


void SV_PixelTable::insert_from(SV_PixelTable& other) {
    for (const auto& other_px : other.table) {
        if (other_px.x != 1) {
            insert(other_px);
        }
    }
}


bool SV_PixelTable::is_empty() {return empty;}


int SV_PixelTable::hash(int x, int y) {
    if (x < y) {
        return y*y + x;
    }
    else {
        return x*x + x + y;
    }
}

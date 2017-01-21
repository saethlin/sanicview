#ifndef SANICVIEW_SV_PIXELTABLE_H
#define SANICVIEW_SV_PIXELTABLE_H

#include <vector>
#include "pixel.h"

class SV_PixelTable {
public:
    SV_PixelTable() {}
    SV_PixelTable(int max_val1, int max_val2);
    void insert(pixel px);
    void clear();
    void insert_from(SV_PixelTable& other);
    bool is_empty();

private:
    int hash(int x, int y);
    std::vector<pixel> table;
    bool empty = false;
};



#endif //SANICVIEW_SV_PIXELTABLE_H

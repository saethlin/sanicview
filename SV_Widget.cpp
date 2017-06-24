#include "SV_Widget.h"
#include "SV_Window.h"


SV_Widget::SV_Widget(SV_Window *window, int x, int y, int width, int height) {
    this->x(x);
    this->y(y);
    this->w(width);
    this->h(height);
    this->parent_window = window;
    window->add(this);
}


void SV_Widget::draw_point(int pixel_x, int pixel_y, uint8_t r, uint8_t g, uint8_t b) {
    window()->draw_point(x() + pixel_x, y() + pixel_y, r, g, b);
}


void SV_Widget::draw_text(std::string text, int x, int y, int pt) {
    window()->draw_text(text, x + x_impl, y + y_impl, pt);
}

const std::array<color, 256> inferno =
    {{{0, 0, 4},
    {1, 0, 5},
    {1, 1, 6},
    {1, 1, 8},
    {2, 1, 10},
    {2, 2, 12},
    {2, 2, 14},
    {3, 2, 16},
    {4, 3, 18},
    {4, 3, 20},
    {5, 4, 23},
    {6, 4, 25},
    {7, 5, 27},
    {8, 5, 29},
    {9, 6, 31},
    {10, 7, 34},
    {11, 7, 36},
    {12, 8, 38},
    {13, 8, 41},
    {14, 9, 43},
    {16, 9, 45},
    {17, 10, 48},
    {18, 10, 50},
    {20, 11, 52},
    {21, 11, 55},
    {22, 11, 57},
    {24, 12, 60},
    {25, 12, 62},
    {27, 12, 65},
    {28, 12, 67},
    {30, 12, 69},
    {31, 12, 72},
    {33, 12, 74},
    {35, 12, 76},
    {36, 12, 79},
    {38, 12, 81},
    {40, 11, 83},
    {41, 11, 85},
    {43, 11, 87},
    {45, 11, 89},
    {47, 10, 91},
    {49, 10, 92},
    {50, 10, 94},
    {52, 10, 95},
    {54, 9, 97},
    {56, 9, 98},
    {57, 9, 99},
    {59, 9, 100},
    {61, 9, 101},
    {62, 9, 102},
    {64, 10, 103},
    {66, 10, 104},
    {68, 10, 104},
    {69, 10, 105},
    {71, 11, 106},
    {73, 11, 106},
    {74, 12, 107},
    {76, 12, 107},
    {77, 13, 108},
    {79, 13, 108},
    {81, 14, 108},
    {82, 14, 109},
    {84, 15, 109},
    {85, 15, 109},
    {87, 16, 110},
    {89, 16, 110},
    {90, 17, 110},
    {92, 18, 110},
    {93, 18, 110},
    {95, 19, 110},
    {97, 19, 110},
    {98, 20, 110},
    {100, 21, 110},
    {101, 21, 110},
    {103, 22, 110},
    {105, 22, 110},
    {106, 23, 110},
    {108, 24, 110},
    {109, 24, 110},
    {111, 25, 110},
    {113, 25, 110},
    {114, 26, 110},
    {116, 26, 110},
    {117, 27, 110},
    {119, 28, 109},
    {120, 28, 109},
    {122, 29, 109},
    {124, 29, 109},
    {125, 30, 109},
    {127, 30, 108},
    {128, 31, 108},
    {130, 32, 108},
    {132, 32, 107},
    {133, 33, 107},
    {135, 33, 107},
    {136, 34, 106},
    {138, 34, 106},
    {140, 35, 105},
    {141, 35, 105},
    {143, 36, 105},
    {144, 37, 104},
    {146, 37, 104},
    {147, 38, 103},
    {149, 38, 103},
    {151, 39, 102},
    {152, 39, 102},
    {154, 40, 101},
    {155, 41, 100},
    {157, 41, 100},
    {159, 42, 99},
    {160, 42, 99},
    {162, 43, 98},
    {163, 44, 97},
    {165, 44, 96},
    {166, 45, 96},
    {168, 46, 95},
    {169, 46, 94},
    {171, 47, 94},
    {173, 48, 93},
    {174, 48, 92},
    {176, 49, 91},
    {177, 50, 90},
    {179, 50, 90},
    {180, 51, 89},
    {182, 52, 88},
    {183, 53, 87},
    {185, 53, 86},
    {186, 54, 85},
    {188, 55, 84},
    {189, 56, 83},
    {191, 57, 82},
    {192, 58, 81},
    {193, 58, 80},
    {195, 59, 79},
    {196, 60, 78},
    {198, 61, 77},
    {199, 62, 76},
    {200, 63, 75},
    {202, 64, 74},
    {203, 65, 73},
    {204, 66, 72},
    {206, 67, 71},
    {207, 68, 70},
    {208, 69, 69},
    {210, 70, 68},
    {211, 71, 67},
    {212, 72, 66},
    {213, 74, 65},
    {215, 75, 63},
    {216, 76, 62},
    {217, 77, 61},
    {218, 78, 60},
    {219, 80, 59},
    {221, 81, 58},
    {222, 82, 56},
    {223, 83, 55},
    {224, 85, 54},
    {225, 86, 53},
    {226, 87, 52},
    {227, 89, 51},
    {228, 90, 49},
    {229, 92, 48},
    {230, 93, 47},
    {231, 94, 46},
    {232, 96, 45},
    {233, 97, 43},
    {234, 99, 42},
    {235, 100, 41},
    {235, 102, 40},
    {236, 103, 38},
    {237, 105, 37},
    {238, 106, 36},
    {239, 108, 35},
    {239, 110, 33},
    {240, 111, 32},
    {241, 113, 31},
    {241, 115, 29},
    {242, 116, 28},
    {243, 118, 27},
    {243, 120, 25},
    {244, 121, 24},
    {245, 123, 23},
    {245, 125, 21},
    {246, 126, 20},
    {246, 128, 19},
    {247, 130, 18},
    {247, 132, 16},
    {248, 133, 15},
    {248, 135, 14},
    {248, 137, 12},
    {249, 139, 11},
    {249, 140, 10},
    {249, 142, 9},
    {250, 144, 8},
    {250, 146, 7},
    {250, 148, 7},
    {251, 150, 6},
    {251, 151, 6},
    {251, 153, 6},
    {251, 155, 6},
    {251, 157, 7},
    {252, 159, 7},
    {252, 161, 8},
    {252, 163, 9},
    {252, 165, 10},
    {252, 166, 12},
    {252, 168, 13},
    {252, 170, 15},
    {252, 172, 17},
    {252, 174, 18},
    {252, 176, 20},
    {252, 178, 22},
    {252, 180, 24},
    {251, 182, 26},
    {251, 184, 29},
    {251, 186, 31},
    {251, 188, 33},
    {251, 190, 35},
    {250, 192, 38},
    {250, 194, 40},
    {250, 196, 42},
    {250, 198, 45},
    {249, 199, 47},
    {249, 201, 50},
    {249, 203, 53},
    {248, 205, 55},
    {248, 207, 58},
    {247, 209, 61},
    {247, 211, 64},
    {246, 213, 67},
    {246, 215, 70},
    {245, 217, 73},
    {245, 219, 76},
    {244, 221, 79},
    {244, 223, 83},
    {244, 225, 86},
    {243, 227, 90},
    {243, 229, 93},
    {242, 230, 97},
    {242, 232, 101},
    {242, 234, 105},
    {241, 236, 109},
    {241, 237, 113},
    {241, 239, 117},
    {241, 241, 121},
    {242, 242, 125},
    {242, 244, 130},
    {243, 245, 134},
    {243, 246, 138},
    {244, 248, 142},
    {245, 249, 146},
    {246, 250, 150},
    {248, 251, 154},
    {249, 252, 157},
    {250, 253, 161},
    {252, 255, 164}}};

void SV_Widget::draw_point(int pixel_x, int pixel_y, uint8_t color) {
    auto mapped = inferno[color];
    window()->draw_point(x() + pixel_x, y() + pixel_y, mapped.r, mapped.g, mapped.b);
}

#include "SV_MiniMap.h"
#include "SV_Image.h"
#include <cmath>
#include <iostream>


SV_MiniMap::SV_MiniMap(SV_Window* window) : SV_Widget(window, window->w()-200, 0, 200, 200) {}


void SV_MiniMap::set_imagedisplay(SV_Image* imagedisplay) {
    this->imagedisplay = imagedisplay;
}


void SV_MiniMap::set_image(CImg<double>& image) {
    original_width = image.width();
    original_height = image.height();
    this->image = image.get_resize(200, 200);
    clip = true;
    move = true;
    redraw();
}


void SV_MiniMap::set_white(double white) {
    this->white = white;
    clip = true;
    redraw();
}


void SV_MiniMap::set_black(double black) {
    this->black = black;
    clip = true;
    redraw();
}


void SV_MiniMap::set_origin(int new_x, int new_y) {
    if (new_x != x or new_y != y) {
        x = new_x;
        y = new_y;
        move = true;
        redraw();
    }
}


void SV_MiniMap::draw() {
    if (this->image.size() == 0) {return;}

    if (clip) {
        clipped = (image.get_cut(black, white) - black).normalize(0, 255);
        clip = false;
        for (auto y = 0; y < h(); y++) {
            for (auto x = 0; x < w(); x++) {
                draw_point(x, y, clipped(x, y));
            }
        }
    }

    if (move) {

        for (auto x = x0_border; x < x1_border; x++) {
            draw_point(x, y0_border, clipped(x, y0_border));
            draw_point(x, y1_border, clipped(x, y1_border));
        }
        for (auto y = y0_border; y < y1_border; y++) {
            draw_point(x0_border, y, clipped(x0_border, y));
            draw_point(x1_border, y, clipped(x1_border, y));
        }

        x0_border = floor((double) x * 200 / original_width);
        y0_border = floor((double) y * 200 / original_height);
        x1_border = floor(x0_border + ((window()->w() - 200.0) * 200 / original_width)) - 1;
        y1_border = floor(y0_border + ((window()->h() - 50) * 200 / original_height)) - 1;

        for (auto x = x0_border; x < x1_border; x++) {
            draw_point(x, y0_border, 255, 0, 0);
            draw_point(x, y1_border, 255, 0, 0);
        }
        for (auto y = y0_border; y < y1_border; y++) {
            draw_point(x0_border, y, 255, 0, 0);
            draw_point(x1_border, y, 255, 0, 0);
        }
    }
}


bool SV_MiniMap::handle(SV_Event event) {
    if (event.type() == mouse_push) {
        int map_x = (event.x() - 600) * original_width / 200.0 - imagedisplay->w() / 2;
        int map_y = event.y() * original_height / 200.0 - imagedisplay->h() / 2;

        map_x = std::max(0, std::min(map_x, original_width - imagedisplay->w()));
        map_y = std::max(0, std::min(map_y, original_height - imagedisplay->h()));

        set_origin(map_x, map_y);

        imagedisplay->set_origin(map_x, map_y);
        return true;
    }
    return false;
}



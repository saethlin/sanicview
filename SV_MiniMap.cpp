#include "SV_MiniMap.h"
#include "SV_Event.h"
#include "SV_Display.h"
#include "SV_Image.h"
#include <cmath>


SV_MiniMap::SV_MiniMap(SV_Window* window) : SV_Widget(window, window->w()-200, 0, 200, 200) {}


void SV_MiniMap::set_imagedisplay(SV_Display* imagedisplay) {
    this->imagedisplay = imagedisplay;
}


void SV_MiniMap::set_image(SV_Image<double>& image) {
    original_width = image.width();
    original_height = image.height();
    this->image = SV_Image<double>(w(), h());
    this->clipped = SV_Image<unsigned char>(w(), h());
    for (auto y = 0; y < h(); y++) {
        for (auto x = 0; x < w(); x++) {
            this->image(x, y) = image(x*image.width()/200, y*image.height()/200);
        }
    }
    // Copy in the appropriate pixels
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
        for (auto i = 0; i < image.size(); i++) {
            clipped[i] = (unsigned char)((clamp(black, image[i], white) - black) * (255/(white-black)));
        }
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


bool SV_MiniMap::handle(const SV_Event& event) {
    if (event.type() == mouse_push) {
        clicked = true;
    }
    if (event.type() == mouse_push or (clicked and event.type() == mouse_move) or (clicked and event.type() == mouse_release)) {
        int map_x = (event.x() - 600) * original_width / 200.0 - imagedisplay->w() / 2;
        int map_y = event.y() * original_height / 200.0 - imagedisplay->h() / 2;

        map_x = std::max(0, std::min(map_x, original_width - imagedisplay->w()));
        map_y = std::max(0, std::min(map_y, original_height - imagedisplay->h()));

        set_origin(map_x, map_y);
        if (event.type() == mouse_release) {
            imagedisplay->set_origin(map_x, map_y);
            clicked = false;
        }
        return true;
    }
    return false;
}



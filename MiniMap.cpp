#include "MiniMap.h"
#include "Event.h"
#include "Display.h"
#include "Image.h"
#include "Window.h"
#include <cmath>


MiniMap::MiniMap(Window* window) : Widget(window, window->w()-200, 0, 200, 200) {}


void MiniMap::set_imagedisplay(Display* imagedisplay) {
    this->imagedisplay = imagedisplay;
}


void MiniMap::set_image(Image<float>& image) {
    original_width = image.width();
    original_height = image.height();
    this->image = Image<float>(w(), h());
    this->clipped = Image<uint8_t>(w(), h());
    for (auto y = 0; y < h(); y++) {
        for (auto x = 0; x < w(); x++) {
            this->image(x, y) = image(x*image.width()/w(), y*image.height()/h());
        }
    }
    // Copy in the appropriate pixels
    clip = true;
    move = true;
    redraw();
}


void MiniMap::set_white(double white) {
    this->white = white;
    clip = true;
    redraw();
}


void MiniMap::set_black(double black) {
    this->black = black;
    clip = true;
    redraw();
}


void MiniMap::set_origin(int new_x, int new_y) {
    if (new_x != view_x or new_y != view_y) {
        view_x = new_x;
        view_y = new_y;
        move = true;
        redraw();
    }
}


void MiniMap::draw() {
    if (this->image.size() == 0) {return;}

    if (clip) {
        for (int i = 0; i < image.size(); i++) {
            clipped[i] = (uint8_t)((clamp(black, image[i], white) - black) * (255/(white-black)));
        }
    }

    if (clip || do_resize) {
        for (int y = 0; y < h(); y++) {
            for (int x = 0; x < w(); x++) {
                draw_point(x, y, clipped(x, y));
            }
        }
    }

    if (clip || move || do_resize) {
        for (int x = x0_border; x < x1_border; x++) {
            draw_point(x, y0_border, clipped(x, y0_border));
            draw_point(x, y1_border, clipped(x, y1_border));
        }
        for (int y = y0_border; y < y1_border; y++) {
            draw_point(x0_border, y, clipped(x0_border, y));
            draw_point(x1_border, y, clipped(x1_border, y));
        }

        x0_border = floor((double) view_x * w() / original_width);
        y0_border = floor((double) view_y * h() / original_height);
        x1_border = floor(x0_border + ((window()->w() - 200.0) * w() / original_width)) - 1;
        y1_border = floor(y0_border + ((window()->h() - 50) * h() / original_height)) - 1;

        for (int x = x0_border; x < x1_border; x++) {
            //draw_point(x, y0_border, 255, 0, 0);
            //draw_point(x, y1_border, 255, 0, 0);
            draw_point(x, y0_border, 255 - clipped(x ,y0_border));
            draw_point(x, y1_border, 255 - clipped(x, y1_border));
        }
        for (int y = y0_border; y < y1_border; y++) {
            //draw_point(x0_border, y, 255, 0, 0);
            //draw_point(x1_border, y, 255, 0, 0);
            draw_point(x0_border, y, 255-clipped(x0_border, y));
            draw_point(x1_border, y, 255-clipped(x1_border, y));
        }
    }
    clip = false;
    do_resize = false;
    move = false;
}


bool MiniMap::handle(const Event& event) {
    if (event.type() == mouse_push) {
        clicked = true;
    }
    if (event.type() == mouse_push or (clicked and event.type() == mouse_move) or (clicked and event.type() == mouse_release)) {
        int map_x = (event.x() - 600) * original_width / w() - imagedisplay->w() / 2;
        int map_y = event.y() * original_height / h() - imagedisplay->h() / 2;

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


void MiniMap::resize() {
    x(window()->w()-200);
    do_resize = true;
    redraw();
}
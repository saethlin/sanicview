#include "SV_Display.h"
#include "SV_Window.h"
#include "SV_Histogram.h"
#include "SV_MiniMap.h"
#include <iostream>


SV_Display::SV_Display(SV_Window* window) : SV_Widget(window, 0, 0, window->w()-200, window->h()-50) {}


void SV_Display::set_image(SV_Image<double>& image) {
    this->image = image;
    this->clipped = SV_Image<unsigned char>(image.width(), image.height());
    set_origin((image.width()-w())/2, (image.height()-w())/2);

    // Initialize with nice black and white clipping values
    std::vector<double> sortable(image.begin(), image.end());
    auto median = sortable.begin() + (sortable.size()/2);
    auto white_level = sortable.begin() + sortable.size()*0.997;
    std::nth_element(sortable.begin(), median, sortable.end());
    std::nth_element(sortable.begin(), white_level, sortable.end());

    set_white(*white_level);
    set_black(*median);

    minimap->set_image(image);
    minimap->set_white(white);
    minimap->set_black(black);
    minimap->set_origin(x_view, y_view);

    histogram->set_image(image);
}


void SV_Display::draw() {
    if (image.size() == 0) {
        for (auto y = 0; y < h(); y++) {
            for (auto x = 0; x < w(); x++) {
                draw_point(x, y, (unsigned char)0);
            }
        }
    }
    else {
        if (clip) {
            for (auto i = 0; i < image.size(); i++) {
                clipped[i] = (unsigned char)((clamp(black, image[i], white) - black) * (255/(white-black)));
            }
            clip = false;
        }

        for (auto y = 0; y < h(); y++) {
            for (auto x = 0; x < w(); x++) {
                draw_point(x, y, clipped(x+x_view, y+y_view));
            }
        }
    }
}


void SV_Display::set_black(double black) {
    if (black != this->black) {
        this->black = black;
        clip = true;
        redraw();
        if (minimap != NULL) {
            minimap->set_black(black);
        }
    }
}


void SV_Display::set_white(double white) {
    if (white != this->white) {
        this->white = white;
        clip = true;
        redraw();
        if (minimap != NULL) {
            minimap->set_white(white);
        }
    }
}


double SV_Display::get_black() {
    return black;
}


double SV_Display::get_white() {
    return white;
}


void SV_Display::set_origin(int x, int y) {

    auto try_x = std::min(image.width() - w(), std::max(x, 0));
    auto try_y = std::min(image.height() - h(), std::max(y, 0));

    if ((try_x != x_view) || (try_y != y_view)) {
        x_view = try_x;
        y_view = try_y;
        move = true;
        redraw();
    }
}


bool SV_Display::handle(SV_Event event) {
    return false;
}


void SV_Display::resize() {
    w(window()->w()-200);
    h(window()->h()-50);
    move = true;
    redraw();
}


void SV_Display::add(SV_Histogram* histogram) {
    this->histogram = histogram;
    histogram->set_imagedisplay(this);
}


void SV_Display::add(SV_MiniMap* minimap) {
    this->minimap = minimap;
    minimap->set_imagedisplay(this);
}


void SV_Display::add(SV_DirList* dirlist) {
    //dirlist->set_imagedisplay(this);
}


void SV_Display::add(SV_CursorTracker* cursortracker) {
    this->cursortracker = cursortracker;
}

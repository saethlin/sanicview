#include "SV_Image.h"
#include "SV_Window.h"
#include "SV_Histogram.h"
#include "SV_MiniMap.h"
#include <iostream>


SV_Image::SV_Image(SV_Window* window) : SV_Widget(window, 0, 0, window->w()-200, window->h()-50) {}


void SV_Image::set_image(CImg<double>& image) {
    this->image = image;
    set_origin((image.width()-w())/2, (image.height()-w())/2);

    // Initialize with nice black and white clipping values
    std::vector<double> sortable(image.data(), image.data() + image.size());
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
    redraw();
}


void SV_Image::draw() {
    if (image.size() == 0) {
        for (auto y = 0; y < h(); y++) {
            for (auto x = 0; x < w(); x++) {
                draw_point(x, y, (unsigned char)0);
            }
        }
    }
    else if (clip or move) {
        if (clip) {
            clipped = (image.get_cut(black, white) - black).normalize(0, 255);
            clip = false;
        }

        for (auto y = 0; y < h(); y++) {
            for (auto x = 0; x < w(); x++) {
                draw_point(x, y, clipped(x+x_view, y+y_view));
            }
        }
    }
}


void SV_Image::set_black(double black) {
    if (black != this->black) {
        this->black = black;
        clip = true;
        redraw();
        if (minimap != NULL) {
            minimap->set_black(black);
        }
    }
}


void SV_Image::set_white(double white) {
    if (white != this->white) {
        this->white = white;
        clip = true;
        redraw();
        if (minimap != NULL) {
            minimap->set_white(white);
        }
    }
}


double SV_Image::get_black() {
    return black;
}


double SV_Image::get_white() {
    return white;
}


void SV_Image::set_origin(int x, int y) {

    auto try_x = std::min(image.width() - w(), std::max(x, 0));
    auto try_y = std::min(image.height() - h(), std::max(y, 0));

    if ((try_x != x_view) || (try_y != y_view)) {
        x_view = try_x;
        y_view = try_y;
        move = true;
        redraw();
    }
}


bool SV_Image::handle(SV_Event event) {
    return false;
}


void SV_Image::resize() {
    w(window()->w()-200);
    h(window()->h()-50);
    move = true;
    redraw();
}


void SV_Image::add(SV_Histogram* histogram) {
    this->histogram = histogram;
    histogram->set_imagedisplay(this);
}


void SV_Image::add(SV_MiniMap* minimap) {
    this->minimap = minimap;
    minimap->set_imagedisplay(this);
}


void SV_Image::add(SV_DirList* dirlist) {
    //dirlist->set_imagedisplay(this);
}


void SV_Image::add(SV_CursorTracker* cursortracker) {
    this->cursortracker = cursortracker;
}

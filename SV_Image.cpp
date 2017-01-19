#include "SV_Image.h"


SV_Image::SV_Image(SV_Window* window) {
    this->x = 0;
    this->y = 0;
    this->width = window->get_width()-200;
    this->height = window->get_height()-50;
}


void SV_Image::set_image(CImg<double>& image) {
    this->image = image;
    set_origin(0, 0);

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
    minimap->set_origin(x, y);

    histogramdisplay->set_image(image);
}


void SV_Image::draw() {
    if (image.size() == 0) {
        for (auto y = 0; y < height; y++) {
            for (auto x = 0; x < width; x++) {
                change_pixel(x, y, 0);
            }
        }
    }
    else if (clip or move) {
        auto old = cropped;

        if (clip) {
            clipped = (image.get_cut(black, white) - black).normalize(0, 255);
            move = true;
        }

        if (move) {
            cropped = clipped.get_crop(x, y, x+width, y+height);
        }
        clip = false;
        move = false;

        for (auto y = 0; y < height; y++) {
            for (auto x = 0; x < width; x++) {
                if (old(x, y) != cropped(x, y) or x >= old.width() or y >= old.height()) {
                    change_pixel(x + this->x, y + this->y, cropped(x, y));
                }
            }
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

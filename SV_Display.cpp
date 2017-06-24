#include "SV_Display.h"
#include "SV_Event.h"
#include "SV_Window.h"
#include "SV_Histogram.h"
#include "SV_MiniMap.h"
#include "SV_Image.h"
#include "SV_Dirlist.h"

SV_Display::SV_Display(SV_Window* window) : SV_Widget(window, 0, 0, window->w()-200, window->h()-50) {}


void SV_Display::draw() {
    if (image.size() == 0) {
        return;
    }
    else {
        if (clip) {
            for (auto i = 0; i < image.size(); i++) {
                clipped[i] = (uint8_t)((clamp(black, image[i], white) - black) * (255/(white-black)));
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


void SV_Display::set_black(const double black) {
    if (black != this->black) {
        this->black = black;
        clip = true;
        redraw();
        if (minimap != NULL) {
            minimap->set_black(black);
        }
    }
}


void SV_Display::set_white(const double white) {
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


void SV_Display::set_origin(const int x, const int y) {

    auto try_x = std::min(image.width() - w(), std::max(x, 0));
    auto try_y = std::min(image.height() - h(), std::max(y, 0));

    if ((try_x != x_view) || (try_y != y_view)) {
        x_view = try_x;
        y_view = try_y;
        redraw();
    }
}


bool SV_Display::handle(const SV_Event& event) {

    if (image.size() == 0) {
        return false;
    }

    if (event.type() == mouse_move) {
        int x_loc = event.x() + x_view;
        int y_loc = event.y() + y_view;
        if ((x_loc < image.width()) && (y_loc < image.height())) {
            cursortracker->set_location(event.x()+x_view, event.y()+y_view, image(event.x()+x_view, event.y()+y_view));
            return true;
        }
        else {
            // Turns out we didn't want the event
            return false;
        }
    }
    else if (event.type() == key_press) {
        if (event.key() == 21) {
            // zoom in
            set_zoom(zoom*2);
            return true;
        }
        if (event.key() == 20) {
            // zoom out
            set_zoom(zoom/2);
            return true;
        }
        // h key
        if (event.key() == 43) {
            SV_Window header_window(800, 486, 60);
            SV_Header header(&header_window, cards);
            header_window.run();
        }
    }
    return false;
}


void SV_Display::set_zoom(int zoom) {
    if (this->zoom != zoom || zoom >= 1 || zoom <= 8 || zoom % 2 == 0) {
        this->zoom = zoom;
        redraw();
    }
}


void SV_Display::resize() {
    int new_w = window()->w()-200;
    int new_h = window()->h()-50;
    if (new_w > w() || new_h > h()) {
        redraw();
    }
    w(new_w);
    h(new_h);
}


void SV_Display::add(SV_Histogram* histogram) {
    this->histogram = histogram;
    histogram->set_imagedisplay(this);
}


void SV_Display::add(SV_MiniMap* minimap) {
    this->minimap = minimap;
    minimap->set_imagedisplay(this);
}


void SV_Display::add(SV_Dirlist* dirlist) {
    dirlist->set_imagedisplay(this);
}


void SV_Display::add(SV_CursorTracker* cursortracker) {
    this->cursortracker = cursortracker;
}


void SV_Display::open(std::string filename, int hdu) {
    fitsfile *fptr;
    char card[FLEN_CARD];
    int status = 0, nkeys;   // CFITSIO status value MUST be initialized to zero

    if (!fits_open_file(&fptr, filename.c_str(), READONLY, &status)) {
        fits_movrel_hdu(fptr, hdu, NULL, &status);
        if (!status) {
            fits_get_hdrspace(fptr, &nkeys, NULL, &status); // get # of keywords
            for (int i = 1; i <= nkeys; i++) { // Read each keywords
                if (!fits_read_record(fptr, i, card, &status)) {
                    cards.push_back({card});
                }
                else {
                    break;
                }
            }

            int bitpix, naxis;
            long naxes[2];
            fits_get_img_param(fptr, 2, &bitpix, &naxis, naxes, &status);
            int n_elements = 1;
            for (int i = 0; i < naxis; i++) {
                n_elements *= naxes[i];
            }

            image = SV_Image<double>(naxes[0], naxes[1]);
            int anynul;
            long fpixel[2] {1, 1};
            fits_read_pix(fptr, TDOUBLE, fpixel, n_elements, NULL, &(*image.begin()), &anynul, &status);
        }
        if (status == END_OF_FILE) {
            status = 0;
        }
        fits_close_file(fptr, &status);
        if (status != 0) {
            throw status;
        }
    }

    if (image.size() == 0) {
        return;
    }
    clipped = SV_Image<uint8_t>(image.width(), image.height());
    set_origin((image.width()-w())/2, (image.height()-h())/2);

    histogram->set_image(image);

    minimap->set_image(image);
    minimap->set_white(white);
    minimap->set_black(black);
    minimap->set_origin(x_view, y_view);
}
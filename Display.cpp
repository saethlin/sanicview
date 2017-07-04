#include "Display.h"
#include "Event.h"
#include "Window.h"
#include "Histogram.h"
#include "MiniMap.h"
#include "Image.h"
#include "DirList.h"
#include "Header.h"
#include "CursorTracker.h"

#include <fitsio.h>

Display::Display(Window* window) : Widget(window, 0, 0, window->w()-200, window->h()-50) {}


void Display::draw() {
    if (image.size() == 0) {
        return;
    }
    else {
        if (clip) {
            for (int i = 0; i < image.size(); i++) {
                clipped[i] = (uint8_t)((clamp(black, image[i], white) - black) * (255/(white-black)));
            }
            clip = false;
        }

        for (int y = 0; y < h(); y++) {
            for (int x = 0; x < w(); x++) {
                draw_point(x, y, clipped(x+x_view, y+y_view));
            }
        }
    }
}


void Display::set_black(const double black) {
    if (black != this->black) {
        this->black = black;
        clip = true;
        redraw();
        if (minimap != NULL) {
            minimap->set_black(black);
        }
    }
}


void Display::set_white(const double white) {
    if (white != this->white) {
        this->white = white;
        clip = true;
        redraw();
        if (minimap != NULL) {
            minimap->set_white(white);
        }
    }
}


void Display::set_origin(const int x, const int y) {
    auto try_x = std::min(image.width() - w(), std::max(x, 0));
    auto try_y = std::min(image.height() - h(), std::max(y, 0));

    if ((try_x != x_view) || (try_y != y_view)) {
        x_view = try_x;
        y_view = try_y;
        redraw();
    }
}


bool Display::handle(const Event& event) {

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
            Window header_window(800, 486);
            Header header(&header_window, cards);
            header_window.run();
        }
    }
    return false;
}


void Display::set_zoom(int zoom) {
    if (this->zoom != zoom && zoom >= 1 && zoom <= 8 && zoom % 2 == 0) {
        this->zoom = zoom;
        redraw();
    }
}


void Display::resize() {
    int new_w = window()->w()-200;
    int new_h = window()->h()-50;
    if (new_w > w() || new_h > h()) {
        redraw();
    }
    w(new_w);
    h(new_h);
}


void Display::add(Histogram* histogram) {
    this->histogram = histogram;
    histogram->set_imagedisplay(this);
}


void Display::add(MiniMap* minimap) {
    this->minimap = minimap;
    minimap->set_imagedisplay(this);
}


void Display::add(DirList* dirlist) {
    dirlist->set_imagedisplay(this);
}


void Display::add(CursorTracker* cursortracker) {
    this->cursortracker = cursortracker;
}


void Display::open(const std::string& filename, int hdu) {
    fitsfile *fptr;
    char card[FLEN_CARD];
    int status = 0, nkeys;

    if (!fits_open_file(&fptr, filename.c_str(), READONLY, &status)) {
        if (current_file != filename) {
            current_file = filename;
            hdulist.clear();
            int hdu_type;
            while ((fits_movrel_hdu(fptr, 1, NULL, &status), !status)) {
                fits_get_hdu_type(fptr, &hdu_type, &status);
                if (status) break;
                switch (hdu_type) {
                    case IMAGE_HDU:
                        hdulist.push_back({"image"});
                    case ASCII_TBL:
                        hdulist.push_back({"ascii"});
                    case BINARY_TBL:
                        hdulist.push_back({"binary"});
                    default:
                        break;
                }
            }
        }

        status = 0; // reset status
        fits_movabs_hdu(fptr, 1, NULL, &status);
        if (!status) {
            fits_get_hdrspace(fptr, &nkeys, NULL, &status);
            for (int i = 1; i <= nkeys; i++) {
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

            image = Image<float>(naxes[0], naxes[1]);
            int anynul;
            long fpixel[2] {1, 1};
            fits_read_pix(fptr, TFLOAT, fpixel, n_elements, NULL, &(*image.begin()), &anynul, &status);
        }
        if (status == END_OF_FILE) {
            status = 0;
        }
        fits_close_file(fptr, &status);
        if (status != 0) {
            throw status;
        }
    }

    for (auto& card: cards) {
        while (card.size() < 80) {
            card.push_back(' ');
        }
    }

    if (image.size() == 0) {
        return;
    }
    clipped = Image<uint8_t>(image.width(), image.height());
    set_origin((image.width()-w())/2, (image.height()-h())/2);

    histogram->set_image(image);

    minimap->set_image(image);
    minimap->set_white(white);
    minimap->set_black(black);
    minimap->set_origin(x_view, y_view);
}
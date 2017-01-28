#include "SV_Histogram.h"
#include "SV_Display.h"
#include <iostream>

SV_Histogram::SV_Histogram(SV_Window* window) : SV_Widget(window, 0, window->h()-50, window->w()-200, 50) {}


void SV_Histogram::set_imagedisplay(SV_Display* imagedisplay) {
    this->imagedisplay = imagedisplay;
}


void SV_Histogram::set_image(SV_Image<double>& image) {
    histogram_to_value.clear();
    // Find the max of the image so that we can know how many bins are required
    auto image_max = std::max_element(image.begin(), image.end());

    // Digitize the image
    std::vector<int> bincount((unsigned long)ceil(*image_max)+1);
    for (auto& val : image) {
        bincount[ceil(val)] += 1;
    }

    // Find the value in the largest bin to normalize the data
    auto max_count = *std::max_element(bincount.begin(), bincount.end());

    // Drop all the zero values
    std::vector<unsigned char> data;
    data.reserve(bincount.size());

    auto black = -1;
    auto white = -1;
    auto num_seen = 0;
    for (auto i = 0; i < bincount.size(); i++) {
        auto val = bincount[i];
        if (val != 0) {
            histogram_to_value.push_back(i);
            data.push_back(50*(double)val/(double)max_count);

            num_seen += val;
            if (black == -1 and num_seen > image.size()/2.) {
                black = i;
                black_slider = data.size();
            }
            if (white == -1 and num_seen > 0.997*image.size()) {
                white = i;
                white_slider = data.size();
            }
        }
    }

    imagedisplay->set_black(black);
    imagedisplay->set_white(white);

    // Create actual histogram image
    histogram = SV_Image<unsigned char>(data.size(), 50);
    for (auto x = 0; x < data.size(); x++) {
        for (auto y = 0; y < 50-data[x]; y++) {
            histogram(x, y) = 255;
        }
    }
    redraw();
}


void SV_Histogram::draw() {
    if (scaled.width() != w()) {
        scaled = SV_Image<unsigned char>(w(), h());
        for (auto y = 0; y < h(); y++) {
            for (auto x = 0; x < w(); x++) {
                scaled(x, y) = histogram(x*histogram.width()/w(), y*histogram.height()/h());
            }
        }

        black_pos = black_slider * scaled.width() / histogram.width();
        white_pos = white_slider * scaled.width() / histogram.width();
        new_black_pos = black_pos;
        new_white_pos = white_pos;

        for (auto y = 0; y < h(); y++) {
            for (auto x = 0; x < w(); x++) {
                if ((x == black_pos) or (x == white_pos)) {
                    draw_point(x, y, 255, 0, 0);
                }
                else {
                    draw_point(x, y, scaled(x, y));
                }
            }
        }


    }
    else {
        if (new_black_pos != black_pos) {
            for (auto y = 0; y < h(); y++) {
                draw_point(black_pos, y, scaled(black_pos, y));
                draw_point(new_black_pos, y, 255, 0, 0);
            }
            black_pos = new_black_pos;
        }

        if (new_white_pos != white_pos) {
            for (auto y = 0; y < h(); y++) {
                draw_point(white_pos, y, scaled(white_pos, y));
                draw_point(new_white_pos, y, 255, 0, 0);
            }
            white_pos = new_white_pos;
        }
    }
}


bool SV_Histogram::handle(SV_Event event) {
    switch (event.type()) {
        case mouse_push: {
            if (abs(event.x() - white_pos) < 4) {
                clicked = WHITE;
                return true;
            }
            else if (abs(event.x() - black_pos) < 4) {
                clicked = BLACK;
                return true;
            }
            break;
        }
        case mouse_move: {
            auto cursor_pos = event.x();
            if (clicked == WHITE) {
                new_white_pos = std::min(w()-1, std::max(black_pos+1, cursor_pos));
                redraw();
                return true;
            } else if (clicked == BLACK) {
                new_black_pos = std::min(white_pos-1, std::max(1, cursor_pos));
                redraw();
                return true;
            }
            break;
        }
        case mouse_release: {
            if (clicked == BLACK) {
                black_slider = (double) black_pos * (double) histogram.width() / (double) scaled.width();
                imagedisplay->set_black(histogram_to_value[(int) black_slider]);
                clicked = NONE;
                return true;
            } else if (clicked == WHITE) {
                white_slider = (double) white_pos * (double) histogram.width() / (double) scaled.width();
                imagedisplay->set_white(histogram_to_value[(int) white_slider]);
                clicked = NONE;
                return true;
            }
            break;
        }
        default:
            break;
    }
    return false;
}

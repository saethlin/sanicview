#include "SV_Histogram.h"
#include "SV_Image.h"
#include <iostream>


enum ClickState {BLACK, WHITE, NONE};


SV_Histogram::SV_Histogram(SV_Window* window) : SV_Widget(window, 0, window->h()-50, window->w()-200, 50) {}


void SV_Histogram::set_imagedisplay(SV_Image* imagedisplay) {
    this->imagedisplay = imagedisplay;
}


void SV_Histogram::set_image(CImg<double>& image) {
    std::vector<double> sortable(image.data(), image.data() + image.size());
    auto image_max = std::max_element(sortable.begin(), sortable.end());

    std::vector<int> bincount((unsigned long)(ceil(*image_max)+1));
    for (auto& val : sortable) {
        bincount[ceil(val)] += 1;
    }

    //std::vector<double> histogram_to_value;

    auto max_counts = std::max_element(bincount.begin(), bincount.end());
    auto max_count = *max_counts;

    auto data = std::vector<unsigned char>();
    data.reserve(bincount.size());

    double closest_black_distance = imagedisplay->get_black();
    double closest_white_distance = imagedisplay->get_white();
    black_slider = 0.0;
    white_slider = 0.0;

    for (auto i = 0; i < bincount.size(); i++) {
        auto val = bincount[i];
        if (val != 0) {

            histogram_to_value.push_back(i);
            data.push_back(50*(double)val/(double)max_count);

            auto test_black = fabs(i - imagedisplay->get_black());
            if (test_black < closest_black_distance) {
                closest_black_distance = test_black;
                black_slider = data.size();
            }

            auto test_white = fabs(i - imagedisplay->get_white());
            if (test_white < closest_white_distance) {
                closest_white_distance = test_white;
                white_slider = data.size();
            }

        }
    }

    // Create actual histogram image
    histogram = CImg<unsigned char>(data.size(), 50, 1, 1, 255);
    for (auto x = 0; x < data.size(); x++) {
        for (auto y = 50-data[x]; y < 50; y++) {
            histogram(x, y) = 0;
        }
    }

    clicked = NONE;

    redraw();

}


void SV_Histogram::draw() {
    if (scaled.width() != w()) {
        scaled = histogram.get_resize(w(), h());
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

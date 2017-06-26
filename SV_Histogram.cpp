#include "SV_Histogram.h"
#include "SV_Event.h"
#include "SV_Display.h"
#include "SV_Window.h"


SV_Histogram::SV_Histogram(SV_Window* window) : SV_Widget(window, 0, window->h()-50, window->w()-200, 50) {}


void SV_Histogram::set_imagedisplay(SV_Display* imagedisplay) {
    this->imagedisplay = imagedisplay;
}


void SV_Histogram::set_image(SV_Image<float>& image) {
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
    std::vector<uint8_t> data;
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
                black_level = data.size();
            }
            if (white == -1 and num_seen > 0.997*image.size()) {
                white = i;
                white_level = data.size();
            }
        }
    }

    imagedisplay->set_black(black);
    imagedisplay->set_white(white);

    // Create actual histogram image
    histogram = SV_Image<uint8_t>(data.size(), 50);
    for (auto x = 0; x < data.size(); x++) {
        for (auto y = 0; y < 50-data[x]; y++) {
            histogram(x, y) = 255;
        }
    }

    black_pos = black_level * w() / histogram.width();
    white_pos = white_level * w() / histogram.width();

    redraw();
}


void SV_Histogram::draw() {
    if (histogram.size() == 0) {
        return;
    }
    if (scaled.width() != w()) {
        scaled = SV_Image<uint8_t>(w(), h());
        for (auto y = 0; y < h(); y++) {
            for (auto x = 0; x < w(); x++) {
                scaled(x, y) = histogram(x * histogram.width() / w(), y * histogram.height() / h());
            }
        }
        for (auto y = 0; y < h(); y++) {
            for (auto x = 0; x < w(); x++) {
                draw_point(x, y, scaled(x, y), scaled(x, y), scaled(x, y));
            }
        }
    }

    for (auto y = 0; y < h(); y++) {
        draw_point(last_black, y, scaled(last_black, y), scaled(last_black,y), scaled(last_black,y));
        draw_point(last_white, y, scaled(last_white, y), scaled(last_white,y), scaled(last_white,y));
    }
    for (auto y = 0; y < h(); y++) {
        draw_point(black_pos, y, 255, 0, 0);
        draw_point(white_pos, y, 255, 0, 0);
    }
    last_black = black_pos;
    last_white = white_pos;
}


bool SV_Histogram::handle(const SV_Event& event) {
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
                white_pos = std::min(w()-1, std::max(black_pos+1, cursor_pos));
                redraw();
                return true;
            } else if (clicked == BLACK) {
                black_pos = std::min(white_pos-1, std::max(1, cursor_pos));
                redraw();
                return true;
            }
            break;
        }
        case mouse_release: {
            if (clicked == BLACK) {
                black_level = (double) black_pos * (double) histogram.width() / (double) scaled.width();
                imagedisplay->set_black(histogram_to_value[(int) black_level]);
                clicked = NONE;
                return true;
            } else if (clicked == WHITE) {
                white_level = (double) white_pos * (double) histogram.width() / (double) scaled.width();
                imagedisplay->set_white(histogram_to_value[(int) white_level]);
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


void SV_Histogram::resize() {
    y(window()->h()-50);
    w(window()->w()-200);
    redraw();
}
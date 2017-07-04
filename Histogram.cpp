#include "Histogram.h"
#include "Event.h"
#include "Display.h"
#include "Window.h"
#include <algorithm>


Histogram::Histogram(Window* window) : Widget(window, 0, window->h()-50, window->w()-200, 50) {}


void Histogram::set_imagedisplay(Display* imagedisplay) {
    this->imagedisplay = imagedisplay;
}


void Histogram::set_image(const Image<float>& image) {
    histogram_to_value.clear();
    data.clear();
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
            if (black == -1 && num_seen > image.size()/2.) {
                black = i;
                black_level = data.size();
            }
            if (white == -1 && num_seen > 0.997*image.size()) {
                white = i;
                white_level = data.size();
            }
        }
    }
    data.shrink_to_fit();

    imagedisplay->set_black(black);
    imagedisplay->set_white(white);

    black_pos = black_level * w() / data.size();
    white_pos = white_level * w() / data.size();

    draw_rectangle(0, 0, w(), h(), 16777215);
    for (int x = 0; x < w(); x++) {
        int data_x = x * data.size() / w();
        if (data[data_x] != 0) {
            draw_line(x, h()-data[data_x], x, h(), 0);
        }
    }
    draw_line(black_pos, 0, black_pos, h(), 16711680);
    draw_line(white_pos, 0, white_pos, h(), 16711680);
}


void Histogram::draw() {
    if (data.empty()) {
        return;
    }

    if (black_pos != last_black) {
        draw_line(last_black, 0, last_black, h(), 16777215);
        draw_line(last_black, h()-data[last_black * data.size() / w()], last_black, h(), 0);
        draw_line(black_pos, 0, black_pos, h(), 16711680);
    }
    if (white_pos != last_white) {
        draw_line(last_white, 0, last_white, h(), 16777215);
        draw_line(last_white, h()-data[last_white * data.size() / w()], last_white, h(), 0);
        draw_line(white_pos, 0, white_pos, h(), 16711680);
    }

    last_black = black_pos;
    last_white = white_pos;
}


bool Histogram::handle(const Event& event) {
    switch (event.type()) {
        case mouse_push: {
            if (abs(event.x() - white_pos) < 4) {
                clicked = WHITE;
            }
            else if (abs(event.x() - black_pos) < 4) {
                clicked = BLACK;
            }
            return true;
        }
        case mouse_move: {
            if (clicked == WHITE) {
                white_pos = std::min(w()-1, std::max(black_pos+1, event.x()));
                redraw();
            } else if (clicked == BLACK) {
                black_pos = std::min(white_pos-1, std::max(1, event.x()));
                redraw();
            }
            return true;
        }
        case mouse_release: {
            if (clicked == BLACK) {
                black_level = (double) black_pos * data.size() / w();
                imagedisplay->set_black(histogram_to_value[(int) black_level]);
            } else if (clicked == WHITE) {
                white_level = (double) white_pos * data.size() / w();
                imagedisplay->set_white(histogram_to_value[(int) white_level]);
            }
            clicked = NONE;
            return true;
        }
        default:
            return false;
    }
}


void Histogram::resize() {
    y(window()->h()-50);
    w(window()->w()-200);
    redraw();
}
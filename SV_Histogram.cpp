#include "SV_Histogram.h"
#include "SV_Image.h"
#include <iostream>
#include <xcb/xcb.h>


SV_Histogram::SV_Histogram(SV_Window* window) : SV_Widget(window, 0, window->h()-50, window->w()-200, 50) {}


void SV_Histogram::set_imagedisplay(SV_Image* imagedisplay) {
    this->imagedisplay = imagedisplay;
}


void SV_Histogram::set_image(CImg<double>& image) {
    std::vector<double> sortable(image.data(), image.data() + image.size());
    auto image_max = std::max_element(sortable.begin(), sortable.end());

    std::vector<int> bincount((unsigned long)(ceil(*image_max)));
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
    const unsigned char red[3] = {255, 0, 0};

    if (this->histogram.size() == 0) {
        //fl_draw_box(FL_FLAT_BOX, 0, this->parent()->h() - h(), w(), h(), fl_rgb_color(255));
    }
    else {
        auto old = scaled;

        if (scaled.width() != w()) {
            scaled = histogram.get_resize(w(), h(), 1, 3);

            black_pos = black_slider * scaled.width() / histogram.width();
            white_pos = white_slider * scaled.width() / histogram.width();
            new_black_pos = black_pos;
            new_white_pos = white_pos;

            black_column = scaled.get_column(black_pos);
            white_column = scaled.get_column(white_pos);

            scaled.draw_line(black_pos, 0, black_pos, h(), red);
            scaled.draw_line(white_pos, 0, white_pos, h(), red);

        }
        else {
            if (new_black_pos != black_pos) {
                scaled.draw_image(black_pos, 0, 0, 0, black_column);
                scaled.draw_line(black_pos, 0, black_pos, h(), red);
                black_column = scaled.get_column(new_black_pos);
                black_pos = new_black_pos;
            }
            if (new_white_pos != white_pos) {
                scaled.draw_image(white_pos, 1, 1, 1, white_column);
                scaled.draw_line(new_white_pos, 0, new_white_pos, h(), red);
                white_column = scaled.get_column(new_white_pos);
                white_pos = new_white_pos;
            }
        }

        for (auto y = 0; y < h(); y++) {
            for (auto x = 0; x < w(); x++) {
                if (x >= old.width() or y >= old.height() or old(x, y) != scaled(x, y)) {
                    const unsigned char color[3] = {scaled(x, y, 0, 0), scaled(x, y, 0, 1), scaled(x, y, 0, 2)};
                    change_pixel(x + this->x(), y + this->y(), color);
                }
            }
        }

        if (get_changed_pixels().size() != 0) {
            std::cout << get_changed_pixels().size() << std::endl;
        }

    }
}


bool SV_Histogram::handle(xcb_generic_event_t* event) {
    switch (event->response_type) {
        case XCB_BUTTON_PRESS: {
            xcb_button_press_event_t* bp = (xcb_button_press_event_t*)event;
            if (abs(bp->event_x - white_pos) < 4) {
                clicked = WHITE;
                return true;
            }
            else if (abs(bp->event_x - black_pos) < 4) {
                clicked = BLACK;
                return true;
            }
            break;
        }
        case XCB_MOTION_NOTIFY: {
            xcb_motion_notify_event_t *motion = (xcb_motion_notify_event_t *) event;
            auto cursor_pos = motion->event_x;
            if (clicked == WHITE) {
                if (cursor_pos > black_pos and cursor_pos < w()) {
                    new_white_pos = cursor_pos;
                    redraw();
                    return true;
                }
            } else if (clicked == BLACK) {
                if (cursor_pos < white_pos and cursor_pos > 0) {
                    new_black_pos = cursor_pos;
                    redraw();
                    return true;
                }
            }
            break;
        }
        case XCB_BUTTON_RELEASE: {
            if (clicked == BLACK) {
                black_slider = (double) black_pos * (double) histogram.width() / (double) scaled.width();
                imagedisplay->set_black(histogram_to_value[(int) black_slider]);
                clicked = NONE;
                auto dis = CImgDisplay();
                scaled.display(dis);
                dis.show();
                sleep(5);
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

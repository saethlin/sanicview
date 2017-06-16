#include "SV_Header.h"


SV_Header::SV_Header(SV_Window* window, std::vector<std::string> cards):
SV_Widget(window, 0, 0, window->w(), window->h()) {
    this->cards = cards;
    display_cards.reserve(cards.size());
    for (auto& s : cards) {
        display_cards.push_back(&s);
    }
}


void SV_Header::resize() {
    x(0);
    y(0);
    w(window()->w());
    h(window()->h());
    redraw();
}


void SV_Header::draw() {
    for (int x = 0; x < w(); x++) {
        for (int y = 0; y < h(); y++) {
            draw_point(x, y, 0);
        }
    }

    for (int i = 0; i < display_cards.size(); i++) {
        if ((spacing*(i + 1)) > h()) {
            break;
        }
        draw_text(*display_cards[i+display_start], 0, spacing*(i+1), 12);
    }
}


bool SV_Header::handle(const SV_Event& event) {
    if (event.type() == key_press) {
        // up arrow
        if (event.key() == 111) {
            if (display_start > 0) {
                display_start -= 1;
                redraw();
            }
            return true;
        }
        // down arrow
        else if (event.key() == 116) {
            if (display_cards.size() - display_start > h()/spacing) {
                display_start += 1;
                redraw();
            }
            return true;
        }
        else if (isprint(event.key())) {
            pattern.push_back(event.key());
            update_matches();
            redraw();
            return true;
        }
        else if (event.key() == 8) {
            pattern.pop_back();
            update_matches();
            redraw();
            return true;
        }
    }
    return false;
}


void SV_Header::update_matches() {
    std::regex re(pattern);
    for (auto& c : cards) {
        if (std::regex_search(c, re)) {
            display_cards.push_back(&c);
        }
    }
}

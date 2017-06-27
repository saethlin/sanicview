#include "Header.h"

Header::Header(Window* window, std::vector<std::string> cards):
Widget(window, 0, 0, window->w(), window->h()) {
    this->cards = cards;
    display_cards.reserve(cards.size());
    for (auto& c : this->cards) {
        display_cards.push_back(&c);
    }
    redraw();
}


void Header::resize() {
    x(0);
    y(0);
    w(window()->w());
    h(window()->h());
    redraw();
}


void Header::draw() {
    for (int y = 0; y < h(); y++) {
        for (int x = 0; x < w(); x++) {
            draw_point(x, y, 0, 0, 0);
        }
    }

    int displayable_cards = (h()-spacing-3)/spacing;
    for (int i = 0; i < std::min(displayable_cards, (int)display_cards.size()); i++) {
        draw_text(*(display_cards[i+display_start]), 0, spacing*(i+1) - 3);
    }
    double segment_size = h()/((double)cards.size());
    int bar_start = floor(segment_size * display_start);
    int bar_end = ceil(segment_size * displayable_cards) + bar_start;
    if (display_cards.size() == 0 || displayable_cards >= display_cards.size()) {
        bar_start = 0;
        bar_end = h();
    }
    for (int y = bar_start; y < bar_end; y++) {
        draw_point(w()-1, y, 255, 255, 255);
        draw_point(w()-2, y, 255, 255, 255);
        draw_point(w()-3, y, 255, 255, 255);
    }
    // Horizontal bar
    for (int x = 0; x < w(); x++) {
        draw_point(x, h()-spacing-4, 255, 255, 255);
    }
    // Current search term
    draw_text("search: "+pattern, 0, h()-5);
    // Cursor
    for (int y = h()-spacing-3+1; y < h()-1; y++) {
        draw_point((pattern.size()+8)*9, y, 255, 255, 255);
        draw_point((pattern.size()+8)*9+1, y, 255, 255, 255);
    }
}


bool Header::handle(const Event& event) {
    if (event.type() == key_release) {
        if (event.key() == 50 || event.key() == 62) {
            shift = false;
        }
    }
    if (event.type() == key_press) {
        if (event.key() == 50 || event.key() == 62) {
            shift = true;
        }
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
        else if (event.key() == 22) {
            if (!pattern.empty()) {
                pattern.pop_back();
                update_matches();
                redraw();
            }
            return true;
        }
        else {
            char key_char = char_from_keycode(event.key());
            if (key_char) {
                if (shift) {
                    key_char = toupper(key_char);
                }
                pattern.push_back(key_char);
                update_matches();
                redraw();
                display_start = 0;
                return true;
            }
        }
    }
    return false;
}


void Header::update_matches() {
    display_cards.clear();
    if (pattern.empty()) {
        for (auto& c : cards) {
            display_cards.push_back(&c);
        }
        return;
    }
    for (auto& c : cards) {
        if (c.find(pattern) != std::string::npos) {
            display_cards.push_back(&c);
        }
    }
}


char char_from_keycode(int keycode) {
    auto numkeys = "1234567890-=";
    auto toprow = "\tqwertyuiop[]";
    auto midrow = "asdfghjkl;'";
    auto botrow = "zxcvbnm,./";
    if (keycode >= 10 && keycode <= 21) {
        return numkeys[keycode-10];
    }
    if (keycode >= 23 && keycode <= 35) {
        return toprow[keycode-23];
    }
    else if (keycode >= 38 && keycode <= 48) {
        return midrow[keycode-38];
    }
    else if (keycode >= 52 && keycode <= 61) {
        return botrow[keycode-52];
    }
    else if (keycode == 65) {
        return ' ';
    }
    return 0;
}
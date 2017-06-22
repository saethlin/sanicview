#include "SV_Header.h"
#include <iostream>

SV_Header::SV_Header(SV_Window* window, std::vector<std::string> cards):
SV_Widget(window, 0, 0, window->w(), window->h()) {
    this->cards = cards;
    display_cards.reserve(cards.size());
    for (auto& c : this->cards) {
        display_cards.push_back(&c);
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

    for (int i = 0; i < display_cards.size()-display_start; i++) {
        // If next line would run into the horizontal bar
        if ((spacing*(i + 1)) > (h()-spacing-3)) {
            break;
        }
        draw_text(*(display_cards[i+display_start]), 0, spacing*(i+1) - 3, 12);
    }
    // Horizontal bar
    for (int x = 0; x < w(); x++) {
        draw_point(x, h()-spacing-4, 255, 255, 0);
    }
    // Current search term
    draw_text("search> "+pattern, 0, h()-5, 12);
    // Cursor
    for (int y = h()-spacing-3; y < h(); y++) {
        draw_point((pattern.size()+8)*9, y, 255, 0, 0);
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
                pattern.push_back(key_char);
                update_matches();
                redraw();
                return true;
            }
        }
    }
    return false;
}


void SV_Header::update_matches() {
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
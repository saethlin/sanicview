#ifndef SANICVIEW_SV_HEADER_H
#define SANICVIEW_SV_HEADER_H

#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include "Window.h"
#include "Widget.h"

char char_from_keycode(int keycode);

class Header : public Widget {
public:
    Header(Window* window, std::vector<std::string> cards);
    void draw() override;
    void resize() override;
    bool handle(const Event& event) override;

private:
    void update_matches();
    std::vector<std::string> cards;
    std::vector<std::string*> display_cards;
    std::string pattern;
    int display_start = 0;
    const int spacing = 20;
    bool shift = false;
};


#endif

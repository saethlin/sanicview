#ifndef SANICVIEW_SV_HEADER_H
#define SANICVIEW_SV_HEADER_H

#include <vector>
#include <string>
#include <regex>
#include <algorithm>
#include "SV_Window.h"
#include "SV_Widget.h"

char char_from_keycode(int keycode);

class SV_Header : public SV_Widget {
public:
    SV_Header(SV_Window* window, std::vector<std::string> cards);
    void draw() override;
    void resize() override;
    bool handle(const SV_Event& event) override;

private:
    void update_matches();
    std::vector<std::string> cards;
    std::vector<std::string*> display_cards;
    std::string pattern;
    int display_start = 0;
    const int spacing = 20;
};


#endif

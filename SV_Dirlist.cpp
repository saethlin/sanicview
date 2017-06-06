#include "SV_Dirlist.h"


SV_Dirlist::SV_Dirlist(SV_Window* window) : SV_Widget(window, window->w()-200, 200, 200, window->h()-200) {}


void SV_Dirlist::resize() {
    w(200);
    h(window()->h()-200);
    redraw();
}

void SV_Dirlist::draw() {
}
#include "DirList.h"
#include "Window.h"
#include "Display.h"
#include "Event.h"
#include <algorithm>


DirList::DirList(Window* window) : Widget(window, window->w()-200, 237, 200, window->h()-237) {
    current_dir = fs::current_path();
    change_dir(current_dir);
    redraw();
}


void DirList::resize() {
    x(window()->w()-200);
    y(250);
    w(200);
    h(window()->h()-250);
    redraw();
}


bool sorted_lower(const fs::directory_entry& lhs, const fs::directory_entry& rhs) {
    std::string a = lhs.path().filename().generic_string();
    std::string b = rhs.path().filename().generic_string();
    return std::tolower(a[0]=='.' ? a[1] : a[0]) < std::tolower(b[0]=='.' ? b[1] : b[0]);
}


void DirList::draw() {
    int spacing = 20;

    if (selection_index >= (display_start + h()/spacing)) {
        display_start = selection_index - h()/spacing + 1;
    }
    if (selection_index < display_start) {
        display_start = selection_index;
    }

    draw_rectangle(0, 0, w(), h(), 0);

    uint32_t color;
    for (int i = 0; i < std::min((int)entries.size(), 13); i++) {
        if (i+display_start == selection_index) color = 255;
        else color = 16777215;
        draw_text(entries[i + display_start].path().filename().generic_string(), 1, spacing * (i + 1)-5, color);
    }
}


void DirList::change_dir(const fs::path& target_path) {
    current_dir = target_path;
    entries.clear();
    for (const auto& entry : fs::directory_iterator(current_dir)) {
        entries.push_back(entry);
    }
    std::sort(entries.begin(), entries.end(), sorted_lower);
}


bool DirList::handle(const Event& event) {
    if (event.type() == key_press) {
        // left arrow
        if (event.key() == 113 || event.key() == 22) {
            if (current_dir != current_dir.root_name()) {
                change_dir(current_dir.parent_path());
                first = true;
                redraw();
            }
            return true;
        }
        // right arrow
        else if (event.key() == 114 || event.key() == 36) {
            if (fs::is_directory(entries[selection_index])) {
                change_dir(entries[selection_index].path());
                first = true;
                redraw();
            }
            else if (fs::is_regular_file(entries[selection_index])) {
                imagedisplay->open(entries[selection_index].path().generic_string(), 0);
            }
            return true;
        }
        // up arrow
        else if (event.key() == 111) {
            if (selection_index > 0) {
                selection_index -= 1;
            }
            else {
                selection_index = entries.size()-1;
            }
            redraw();
            return true;
        }
        // down arrow
        else if (event.key() == 116) {
            if (selection_index < entries.size()-1) {
                selection_index += 1;
            }
            else {
                selection_index = 0;
            }
            redraw();
            return true;
        }
    }
    return false;
}


void DirList::set_imagedisplay(Display* display) {
    this->imagedisplay = display;
}
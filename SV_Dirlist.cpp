#include "SV_Dirlist.h"


SV_Dirlist::SV_Dirlist(SV_Window* window) : SV_Widget(window, window->w()-200, 250, 200, window->h()-250) {
    current_dir = fs::current_path();
    change_dir(current_dir);
}


void SV_Dirlist::resize() {
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

void SV_Dirlist::draw() {
    for (int i = 0; i < entries.size(); i++) {
        draw_text(entries[i].path().filename().generic_string(), 0, 16*(i+1), 12);
    }
}

void SV_Dirlist::change_dir(const fs::path& target_path) {
    current_dir = target_path;
    entries.clear();
    for (const auto& entry : fs::directory_iterator(current_dir)) {
        entries.push_back(entry);
    }
    std::sort(entries.begin(), entries.end(), sorted_lower);
}

bool SV_Dirlist::handle(const SV_Event& event) {
    if (event.type() == key_press) {
        // left arrow
        if (event.key() == 113) {
            if (current_dir != current_dir.root_name()) {
                change_dir(current_dir.parent_path());
                redraw();
            }
            return true;
        }
        // right arrow
        else if (event.key() == 114) {
            if (fs::is_directory(entries[selection_index])) {
                current_dir = entries[selection_index].path();
                redraw();
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
            if (selection_index < entries.size()-2) {
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

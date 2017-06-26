#ifndef SANICVIEW_SV_DIRLIST_H
#define SANICVIEW_SV_DIRLIST_H

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <experimental/filesystem>
#include "Widget.h"

class Window;
class Display;

namespace fs = std::experimental::filesystem;

class DirList : public Widget {
public:
    DirList(Window* window);
    void draw() override;
    void resize() override;
    bool handle(const Event& event) override;
    void set_imagedisplay(Display*);

private:
    void change_dir(const fs::path& target_path);
    Display* imagedisplay;
    std::vector<fs::directory_entry> entries;
    int selection_index = 0;
    int display_start = 0;
    fs::path current_dir;
    bool first = true;
};


#endif

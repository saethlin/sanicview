#ifndef SANICVIEW_SV_DIRLIST_H
#define SANICVIEW_SV_DIRLIST_H

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <experimental/filesystem>
#include "SV_Widget.h"

class SV_Window;
class SV_Display;

namespace fs = std::experimental::filesystem;

class SV_Dirlist : public SV_Widget {
public:
    SV_Dirlist(SV_Window* window);
    void draw() override;
    void resize() override;
    bool handle(const SV_Event& event) override;
    void set_imagedisplay(SV_Display*);

private:
    void change_dir(const fs::path& target_path);
    SV_Display* imagedisplay;
    std::vector<fs::directory_entry> entries;
    int selection_index = 0;
    int display_start = 0;
    fs::path current_dir;
    bool first = true;
};


#endif

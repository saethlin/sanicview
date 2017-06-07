#ifndef SANICVIEW_SV_DIRLIST_H
#define SANICVIEW_SV_DIRLIST_H

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <experimental/filesystem>
#include "SV_Window.h"
#include "SV_Widget.h"

namespace fs = std::experimental::filesystem;

class SV_Dirlist : public SV_Widget {
public:
    SV_Dirlist(SV_Window* window);
    void draw() override;
    void resize() override;
    bool handle(const SV_Event& event) override;

private:
    void change_dir(const fs::path& target_path);
    std::vector<fs::directory_entry> entries;
    int selection_index = 0;
    int display_start = 0;
    fs::path current_dir;
};


#endif

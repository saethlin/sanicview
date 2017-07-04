#ifndef SANICVIEW_SV_IMAGEWIDGET_H
#define SANICVIEW_SV_IMAGEWIDGET_H

#include "Widget.h"
#include "Image.h"

struct HDU {
    std::string type;
};

class Window;
class Event;
class MiniMap;
class Histogram;
class DirList;
class CursorTracker;

class Display : public Widget {
public:
    Display(Window* window);
    void draw() override;
    bool handle(const Event& event) override;
    void resize() override;
    void set_white(const double white);
    void set_black(const double black);
    void set_origin(const int x, const int  y);
    void add(MiniMap* minimap);
    void add(Histogram* histogramdisplay);
    void add(DirList* dirlist);
    void add(CursorTracker* cursordisplay);
    void set_zoom(int zoom);
    void open(const std::string& filename, int hdu);

private:
    Image<float> image;
    Image<uint8_t> clipped;
    std::vector<std::string> cards;
    std::vector<HDU> hdulist;
    std::string current_file;
    int x_view = 0, y_view = 0;
    int zoom = 1;
    bool clip;
    double black = 0, white = 0;
    MiniMap* minimap = NULL;
    Histogram* histogram = NULL;
    CursorTracker* cursortracker = NULL;

};


#endif

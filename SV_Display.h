#ifndef SANICVIEW_SV_IMAGEWIDGET_H
#define SANICVIEW_SV_IMAGEWIDGET_H

#include "SV_Widget.h"
#include "SV_Image.h"
#include "SV_CursorTracker.h"

class SV_Window;
class SV_Event;
class SV_MiniMap;
class SV_Histogram;
class SV_Dirlist;
class SV_CursorTracker;

class SV_Display : public SV_Widget {
public:
    SV_Display(SV_Window* window);
    void draw() override;
    bool handle(const SV_Event& event) override;
    void resize() override;
    void set_image(SV_Image<double>&);
    void set_white(const double white);
    void set_black(const double black);
    void set_origin(const int x, const int  y);
    void add(SV_MiniMap* minimap);
    void add(SV_Histogram* histogramdisplay);
    void add(SV_Dirlist* dirlist);
    void add(SV_CursorTracker* cursordisplay);
    double get_white();
    double get_black();
    void set_zoom(int zoom);


private:
    SV_Image<double> image;
    SV_Image<unsigned char> clipped;
    int x_view = 0, y_view = 0;
    int zoom = 1;
    bool clip, move;
    double black = 0, white = 0;
    SV_MiniMap* minimap = NULL;
    SV_Histogram* histogram = NULL;
    SV_CursorTracker* cursortracker = NULL;

};


#endif //SANICVIEW_SV_IMAGEWIDGET_H

#ifndef SANICVIEW_SV_IMAGEWIDGET_H
#define SANICVIEW_SV_IMAGEWIDGET_H

#include "SV_Widget.h"

#include <CImg.h>
using namespace cimg_library;

class SV_MiniMap;
class SV_Histogram;
class SV_DirList;
class SV_CursorTracker;

class SV_Image : public SV_Widget {
public:
    SV_Image(SV_Window* window);
    void draw() override;
    bool handle(SV_Event event) override;
    void resize() override;
    void set_image(CImg<double>&);
    void set_white(double white);
    void set_black(double black);
    void set_origin(int x, int  y);
    void add(SV_MiniMap* minimap);
    void add(SV_Histogram* histogramdisplay);
    void add(SV_DirList* dirlist);
    void add(SV_CursorTracker* cursordisplay);
    double get_white();
    double get_black();
    //void set_zoom(double zoom);


private:
    CImg<double> image;
    CImg<unsigned char> clipped;
    CImg<unsigned char> cropped;
    int x_view = 0, y_view = 0;
    bool clip, move;
    double black = 0, white = 0;
    SV_MiniMap* minimap = NULL;
    SV_Histogram* histogram = NULL;
    SV_CursorTracker* cursortracker = NULL;

};


#endif //SANICVIEW_SV_IMAGEWIDGET_H

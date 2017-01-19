#ifndef SANICVIEW_SV_IMAGEWIDGET_H
#define SANICVIEW_SV_IMAGEWIDGET_H

#include "SV_Widget.h"


class SV_Image : public SV_Widget {
public:
    SV_Image(SV_Window* window);
    void set_image(CImg<double>&);
    void set_minimap(SV_MiniMap* minimap);
    void draw();
    void add(SV_MiniMap* minimap);
    void add(SV_Histogram* histogramdisplay);
    void add(SV_DirList* dirlist);
    void add(SV_CursorTracker* cursordisplay);
    void set_white(double white);
    void set_black(double black);
    void set_origin(int x, int  y);
    void set_zoom(double zoom);
    double get_white();
    double get_black();

private:
    CImg<double> image;
    CImg<uchar> clipped;
    CImg<uchar> cropped;
    bool clip, move;
    double black, white;
    int x, y, cursor_x, cursor_y, width, height;
    SV_MiniMap* minimap;
    SV_HistogramWidget* histogramdisplay;
    SV_CursorTracker* cursordisplay;

};


#endif //SANICVIEW_SV_IMAGEWIDGET_H

#ifndef SANICVIEW_SV_DIRLIST_H
#define SANICVIEW_SV_DIRLIST_H

#include "SV_Window.h"
#include "SV_Widget.h"

class SV_Dirlist : public SV_Widget {
public:
    SV_Dirlist(SV_Window* window);
    void draw() override;
    void resize() override;

private:
};


#endif

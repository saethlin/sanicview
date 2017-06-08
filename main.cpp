#include "SV_Window.h"
#include "SV_Display.h"
#include "SV_Histogram.h"
#include "SV_MiniMap.h"
#include "SV_Image.h"
#include "SV_Dirlist.h"
#include "SV_CursorTracker.h"

#include <iostream>
#include <cstring>
#include <valarray>
#include <CCfits/CCfits>
using namespace CCfits;


SV_Image<double> readImage(const char* filename) {
    std::valarray<double> contents;
    FITS pInfile(filename, Read, true);
    PHDU& primary_HDU = pInfile.pHDU();

    primary_HDU.read(contents);

    auto x = primary_HDU.axis(0);
    auto y = primary_HDU.axis(1);

    return SV_Image<double>(&contents[0], x, y);
}


int main(int argc, char* argv[]) {
    int width = 800;
    int height = 500;
    int framerate = 60;

    for (auto i = 2; i < argc-1; i++) {
        if (std::strcmp(argv[i], "-w") == 0) {
            width = std::stoi(argv[i+1]);
        }
        else if (std::strcmp(argv[i], "-h") == 0) {
            height = std::stoi(argv[i+1]);
        }
        else if (std::strcmp(argv[i], "-f") == 0) {
            framerate = std::stoi(argv[i+1]);
        }
    }

    SV_Image<double> image;
    try {image = readImage(argv[1]);
    }
    catch (const std::logic_error&) {
        //std::cout << "Must provide a valid fits file or path" << std::endl;
        //return 1;
    }

    SV_Window window(width, height, framerate);

    SV_Display imagedisplay(&window);
    SV_Histogram histogram(&window);
    SV_MiniMap minimap(&window);
    SV_Dirlist dirlist(&window);
    SV_CursorTracker cursordisplay(&window);

    imagedisplay.add(&histogram);
    imagedisplay.add(&minimap);
    imagedisplay.add(&dirlist);
    imagedisplay.add(&cursordisplay);

    imagedisplay.set_image(image);

    window.run();

    return 0;
}
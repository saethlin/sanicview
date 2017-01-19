#include "SV_Window.h"
#include "SV_Image.h"

#include <valarray>
#include <CCfits/CCfits>
using namespace CCfits;


CImg<double> readImage(const char* filename) {
    std::valarray<double> contents;
    FITS pInfile(filename, Read, true);
    PHDU& primary_HDU = pInfile.pHDU();

    primary_HDU.read(contents);

    auto x = primary_HDU.axis(0);
    auto y = primary_HDU.axis(1);

    return CImg<double>(&contents[0], x, y);
}


int main () {

    auto image = readImage("test.fits");

    auto app = SV_Window(800, 500);

    auto imagedisplay = SV_Image(&app);
    imagedisplay.set_image(image);

    app.run();

    return 0;
}
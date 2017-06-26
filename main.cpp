#include "Window.h"
#include "Display.h"
#include "Histogram.h"
#include "MiniMap.h"
#include "Image.h"
#include "DirList.h"
#include "CursorTracker.h"

#include <cstring>
#include <valarray>


int main(int argc, char* argv[]) {
    int width = 800;
    int height = 500;

    for (auto i = 2; i < argc-1; i++) {
        if (std::strcmp(argv[i], "-w") == 0) {
            width = std::stoi(argv[i+1]);
        }
        else if (std::strcmp(argv[i], "-h") == 0) {
            height = std::stoi(argv[i+1]);
        }
    }

    Window window(width, height);

    Display imagedisplay(&window);
    Histogram histogram(&window);
    MiniMap minimap(&window);
    DirList dirlist(&window);
    CursorTracker cursordisplay(&window);

    imagedisplay.add(&histogram);
    imagedisplay.add(&minimap);
    imagedisplay.add(&dirlist);
    imagedisplay.add(&cursordisplay);

    if (argc > 1) {
        imagedisplay.open(argv[1], 0);
    }
    window.run();

    return 0;
}
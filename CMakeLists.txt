cmake_minimum_required(VERSION 3.7)
project(sanicview)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

set(SOURCE_FILES main.cpp
        Event.cpp Event.h
        PixelTable.cpp PixelTable.h
        Window.cpp Window.h
        Widget.cpp Widget.h
        Display.cpp Display.h
        Histogram.cpp Histogram.h
        MiniMap.cpp MiniMap.h
        Image.h
        Display.cpp Display.h
        DirList.cpp DirList.h
        CursorTracker.cpp CursorTracker.h
        Header.cpp Header.h
        colormaps.h
        fonts.h)

include_directories(/usr/include/freetype2)

link_libraries(X11 xcb xcb-icccm cfitsio stdc++fs pthread Xau Xdmcp)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/usr/local/lib -Wall -Wno-sign-compare -O3 -static -fuse-linker-plugin -fuse-ld=gold -flto -s")

add_executable(sanicview ${SOURCE_FILES})

cmake_minimum_required(VERSION 3.0)
project(sanicview)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

set(SOURCE_FILES main.cpp
        SV_Event.cpp SV_Event.h
        SV_PixelTable.cpp SV_PixelTable.h
        SV_Window.cpp SV_Window.h
        SV_Widget.cpp SV_Widget.h
        SV_Display.cpp SV_Display.h
        SV_Histogram.cpp SV_Histogram.h
        SV_MiniMap.cpp SV_MiniMap.h
        SV_Image.h
        SV_Display.cpp SV_Display.h)

link_libraries(X11 xcb xcb-icccm cfitsio CCfits pthread)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -fno-omit-frame-pointer -funroll-loops")

add_executable(sanicview ${SOURCE_FILES})

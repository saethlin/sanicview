cmake_minimum_required(VERSION 3.6)
project(sanicview)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

set(SOURCE_FILES main.cpp
        SV_Event.cpp SV_Event.h
        SV_Timer.cpp SV_Timer.h
        SV_PixelTable.cpp SV_PixelTable.h
        SV_Window.cpp SV_Window.h
        SV_Widget.cpp SV_Widget.h
        SV_Image.cpp SV_Image.h
        SV_Histogram.cpp SV_Histogram.h)

link_libraries(X11 xcb cfitsio CCfits pthread)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -march=native -fno-omit-frame-pointer -O3 -funroll-loops")

add_executable(sanicview ${SOURCE_FILES})
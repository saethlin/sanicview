#include "SV_Window.h"
#include "SV_Widget.h"
#include "SV_PixelTable.h"
#include <algorithm>
#include <iostream>
#include <xcb/xcb.h>


SV_Window::SV_Window(int width, int height, int framerate) {
    this->width = width;
    this->height = height;
    this->framerate = std::chrono::duration<float, std::milli>(1/(double)framerate);
    // Open the connection to the X server
    connection = xcb_connect(NULL, NULL);
    if (!connection) {
        std::cout << "unable to connect to display" << std::endl;
        exit(1);
    }

    // Get the first screen
    auto setup = xcb_get_setup(connection);
    if (!setup) {
        std::cout << "unable to connect to display" << std::endl;
        exit(1);
    }

    xcb_screen_t* screen = xcb_setup_roots_iterator(setup).data;
    if (!screen) {
        std::cout << "unable to connect to display" << std::endl;
        exit(1);
    }

    // Create black (foreground) graphic context
    xcb_window = screen->root;
    foreground = xcb_generate_id(connection);
    uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES ;
    uint32_t values[2] = {screen->black_pixel, 0};

    xcb_create_gc(connection, foreground, xcb_window, mask, values);

    // Create a window
    xcb_window = xcb_generate_id(connection);

    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    values[0] = screen->black_pixel;
    values[1] = (XCB_EVENT_MASK_EXPOSURE |
                 XCB_EVENT_MASK_BUTTON_PRESS |
                 XCB_EVENT_MASK_BUTTON_RELEASE |
                 XCB_EVENT_MASK_POINTER_MOTION |
                 XCB_EVENT_MASK_BUTTON_MOTION |
                 XCB_EVENT_MASK_KEY_PRESS |
                 XCB_EVENT_MASK_KEY_RELEASE |
                 XCB_EVENT_MASK_RESIZE_REDIRECT |
                 XCB_EVENT_MASK_FOCUS_CHANGE | XCB_EVENT_MASK_OWNER_GRAB_BUTTON
    );

    xcb_create_window(connection,                    /* connection          */
                      XCB_COPY_FROM_PARENT,          /* depth               */
                      xcb_window,                    /* window Id           */
                      screen->root,                  /* parent window       */
                      0, 0,                          /* view_x, y                */
                      width, height,                 /* width, height       */
                      10,                            /* border_width        */
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
                      screen->root_visual,           /* visual              */
                      mask, values);                 /* masks */

    const char* title = "sanicview";
    xcb_change_property(connection,
                        XCB_PROP_MODE_REPLACE,
                        xcb_window,
                        XCB_ATOM_WM_NAME,
                        XCB_ATOM_STRING,
                        8,
                        strlen(title),
                        title);

    // Disable resizing
    xcb_size_hints_t hints;
    xcb_icccm_size_hints_set_min_size(&hints, width, height);
    xcb_icccm_size_hints_set_max_size(&hints, width, height);
    xcb_icccm_set_wm_size_hints(connection, xcb_window, XCB_ATOM_WM_NORMAL_HINTS, &hints);

    drawing_buffer = SV_PixelTable(width, height);

    // Load font
    auto filename = "/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-R.ttf";
    FT_Init_FreeType(&library);
    FT_New_Face(library, filename, 0, &face);
}


void SV_Window::draw_loop() {
    { // Special scope for the lock guard
        std::lock_guard<std::mutex> guard(lock);
        for (const auto& widget : widgets) {
            if (widget->needsdraw()) {
                widget->draw();
                widget->clear_draw();
            }
        }
        flush();
    }
    std::this_thread::sleep_for(framerate);
    thread_alive = false;
}


void SV_Window::run() {
    // Map the window on the screen and flush
    xcb_map_window(connection, xcb_window);
    xcb_flush(connection);

    SV_Widget* has_mouse = NULL;
    xcb_generic_event_t* xcb_event_ptr;
    while ((xcb_event_ptr = xcb_wait_for_event(connection))) {
        SV_Event event(xcb_event_ptr);
        lock.lock();

        // Special case for mouse events; redirect them to any widget that has grabbed the mouse focus
        if (has_mouse && (event.type() == mouse_move || event.type() == mouse_release)) {
            has_mouse->handle(event);
            if (event.type() == mouse_release) {
                has_mouse = NULL;
            }
        }
        else {
            for (const auto& widget : widgets) {
                bool was_handled;
                if ((event.type() == key_press || event.type() == key_release) || (widget->x() < event.x() && event.x() < widget->x() + widget->w() &&
                     widget->y() < event.y() && event.y() < widget->y() + widget->h())) {
                    was_handled = widget->handle(event);
                    if (was_handled && event.type() == mouse_push) {
                        has_mouse = widget;
                    }
                    if (was_handled) {break;}
                }
            }
        }

        // Clean up and close if esc is pressed
        if (event.type() == key_press) {
            if ((int)((xcb_key_press_event_t*)xcb_event_ptr)->detail == 9) {
                free(xcb_event_ptr);
                lock.unlock();
                return;
            }
        }

        // If no thread is alive, check if any widgets need to be redrawn and launch one to draw them
        if (event.type() == expose || (!thread_alive && std::any_of(widgets.begin(), widgets.end(), [](SV_Widget* w){return w->needsdraw();}))) {
            thread_alive = true;
            std::thread draw_thread(&SV_Window::draw_loop, this);
            draw_thread.detach();
        }
        free(xcb_event_ptr);
        lock.unlock();
    }
}


void SV_Window::add(SV_Widget* widget) {
    this->widgets.push_back(widget);
}


void SV_Window::flush() {
    if (drawing_buffer.empty()) {return;}

    auto& changed_pixels = drawing_buffer.get_changed();

    if (changed_pixels.empty()) {return;}

    auto current_color = changed_pixels.front().color;

    for (const auto& px : changed_pixels) {
        if (px.color != current_color) {
            xcb_change_gc(connection, foreground, XCB_GC_FOREGROUND, &current_color);
            xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, xcb_window, foreground, color_run.size(), color_run.data());
            color_run.clear();
            current_color = px.color;
        }
        color_run.push_back({(int16_t)px.x, (int16_t)px.y});
    }

    xcb_change_gc(connection, foreground, XCB_GC_FOREGROUND, &current_color);
    xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, xcb_window, foreground, color_run.size(), color_run.data());
    color_run.clear();

    xcb_flush(connection);
}


void SV_Window::draw_point(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    drawing_buffer.insert(x, y, r, g, b);
}


void SV_Window::draw_point(int x, int y, uint8_t color) {
    draw_point(x, y, color, color, color);
}


void SV_Window::draw_text(std::string text, int x, int y, int pt) {
    int dpi = 100;
    FT_Set_Char_Size(face, pt*64, 0, dpi, 0);

    // Identity matrix transformation, does this even do anything?
    FT_Matrix matrix {0x10000L, 0,
                      0, 0x10000L};
    FT_Vector pen {0, 0};

    for (const auto& chr : text) {
        FT_Set_Transform(face, &matrix, &pen);

        FT_Load_Char(face, chr, FT_LOAD_RENDER);

        draw_bitmap(face->glyph->bitmap, face->glyph->bitmap_left+x, y-face->glyph->bitmap_top);

        pen.x += face->glyph->advance.x;
        pen.y += face->glyph->advance.y;
    }
}


void SV_Window::draw_bitmap(const FT_Bitmap& bitmap, FT_Int x_min, FT_Int y_min) {
    for (FT_Int x = 0; x < bitmap.width; x++) {
        for (FT_Int y = 0; y < bitmap.rows; y++) {
            if (x+x_min < width && y+y_min < height) {
                draw_point(x+x_min, y+y_min, bitmap.buffer[y * bitmap.width + x]);
            }
        }
    }
}


SV_Window::~SV_Window() {
    xcb_disconnect(connection);
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}

#include "SV_Widget.h"
#include "SV_Window.h"


SV_Widget::SV_Widget(SV_Window* window, int x, int y, int width, int height) {
    this->x(x);
    this->y(y);
    this->w(width);
    this->h(height);
    this->parent_window = window;
    window->add(this);

    auto filename = "/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-R.ttf";
    FT_Init_FreeType(&library);
    FT_New_Face(library, filename, 0, &face);
}


void SV_Widget::draw_point(int pixel_x, int pixel_y, unsigned char color) {
    if (pixel_x >= 0 and pixel_x < w() and
        pixel_y >= 0 and pixel_y < h()) {
        window()->draw_point(x()+pixel_x, y()+pixel_y, color, color, color);
    }
}


void SV_Widget::draw_point(int pixel_x, int pixel_y, unsigned char r, unsigned char g, unsigned char b) {
    if (pixel_x >= 0 and pixel_x < w() and
        pixel_y >= 0 and pixel_y < h()) {
        window()->draw_point(x()+pixel_x, y()+pixel_y, r, g, b);
    }
}


void SV_Widget::draw_text(std::string text, int x, int y, int pt) {
    int dpi = 100;
    FT_Set_Char_Size(face, pt*64, 0, dpi, 0);

    // Identity matrix transformation, does this even do anything?
    FT_Matrix matrix {0x10000L, 0,
                      0, 0x10000L};
    FT_Vector pen {0, 0};

    auto slot = face->glyph;
    for (const auto &chr : text) {
        FT_Set_Transform(face, &matrix, &pen);

        FT_Load_Char(face, chr, FT_LOAD_RENDER);

        draw_bitmap(slot->bitmap, slot->bitmap_left+x, y-slot->bitmap_top);

        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }
}


void SV_Widget::draw_bitmap(const FT_Bitmap& bitmap, FT_Int x_min, FT_Int y_min) {
    for (FT_Int x = 0; x < bitmap.width; x++) {
        for (FT_Int y = 0; y < bitmap.rows; y++) {
            draw_point(x+x_min, y+y_min, bitmap.buffer[y * bitmap.width + x]);
        }
    }
}


bool SV_Widget::needsdraw() {
    if (do_redraw) {
        do_redraw = false;
        return true;
    }
    else {
        return false;
    }
}


SV_Widget::~SV_Widget() {
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}

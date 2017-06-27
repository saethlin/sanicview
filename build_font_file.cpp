#include <vector>
#include <iostream>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

int main() {
    auto filename = "UbuntuMono-B.ttf";
    FT_Library library;
    FT_Face face;
    FT_Init_FreeType(&library);
    FT_New_Face(library, filename, 0, &face);

    int pt = 12;
    int dpi = 100;
    FT_Set_Char_Size(face, pt * 64, 0, dpi, 0);

    FT_Matrix matrix{0x10000L, 0,
                     0, 0x10000L};
    FT_Vector pen{0, 0};

    std::cout << "std::vector<glyph> glyphs {" << std::endl;

    for (FT_ULong chr = 0; chr < 256; chr++) {
        FT_Set_Transform(face, &matrix, &pen);

        FT_Load_Char(face, chr, FT_LOAD_RENDER);

        auto map = face->glyph->bitmap;
        auto g = face->glyph;
        std::vector<uint8_t> tmp(map.buffer, map.buffer + (map.width*map.rows));

        std::cout << "{"; // begin glyph
        std::cout << "{"; // begin image
        std::cout << "{"; // begin vector
        for (int i = 0; i < map.width*map.rows; i++) {
            std::cout << (int) map.buffer[i] << ", ";
        }
        std::cout << "}, "; // end vector
        std::cout << map.width << ", " << map.rows << "}, "; // end image
        std::cout << g->bitmap_top << ", " << g->bitmap_left << "},\n"; // end glyph

        pen.x = 0;
        pen.y = 0;
    }
    std::cout << "};\n";
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}

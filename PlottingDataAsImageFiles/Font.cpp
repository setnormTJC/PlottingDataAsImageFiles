#include "Font.h"

Bitmap Font::renderTextToBitmap(const std::string& text, const std::string& fontPath, int fontSize)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        throw std::runtime_error("Could not init FreeType Library");
    }

    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        throw std::runtime_error("Failed to load font");
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    Bitmap bitmap;
    bitmap.width = 0;
    bitmap.height = fontSize;
    for (char c : text) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load Glyph" << std::endl;
            continue;
        }

        bitmap.width += face->glyph->advance.x >> 6; // Use advance width for spacing
    }

    bitmap.buffer.resize(bitmap.width * bitmap.height, 0);

    int xOffset = 0;
    for (char c : text) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            continue;
        }

        FT_Bitmap& glyphBitmap = face->glyph->bitmap;
        for (int y = 0; y < glyphBitmap.rows; ++y) {
            for (int x = 0; x < glyphBitmap.width; ++x) {
                int index = (y * bitmap.width) + x + xOffset;
                bitmap.buffer[index] = glyphBitmap.buffer[y * glyphBitmap.width + x];
            }
        }
        xOffset += face->glyph->advance.x >> 6; // Move xOffset by advance width
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return bitmap;
}


void Font::flipBitmapVertically(Bitmap& bitmap)
{
    for (int y = 0; y < bitmap.height / 2; ++y)
    {
        for (int x = 0; x < bitmap.width; ++x)
        {
            std::swap(bitmap.buffer[y * bitmap.width + x], bitmap.buffer[(bitmap.height - 1 - y) * bitmap.width + x]);

        }
    }
}

Font::Font(const std::string& text, const std::string& fontPath, int fontSize)
{
    bitmap = renderTextToBitmap(text, fontPath, fontSize);
    flipBitmapVertically(bitmap); 
}

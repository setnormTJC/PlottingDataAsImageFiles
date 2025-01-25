#pragma once

#include"freetype-2.13.3/include/ft2build.h"
#include"freetype-2.13.3/include/freetype/freetype.h"
#include <iostream>
#include <vector>

#pragma comment(lib, "freetyped.lib")

struct Bitmap {
    int width;
    int height;
    std::vector<unsigned char> buffer;
};


class Font
{
    void flipBitmapVertically(Bitmap& bitmap);
    Font() = delete; 
public:
    /*N.B. This constructor fills the bitmap*/
    Font(const std::string& text, const std::string& fontPath, int fontSize);

    Bitmap renderTextToBitmap(const std::string& text, const std::string& fontPath, int fontSize);

    Bitmap bitmap;
};


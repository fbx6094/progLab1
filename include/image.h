#pragma once
#include <stdexcept>
#include <vector>

#include "color.h"

class Image {
   private:
    int width = 0;
    int height = 0;
    std::vector<Color> pixels;

    int getIndex(int x, int y) const { return y * width + x; }

   public:
    Image(int w, int h, Color background = Color(0, 0, 0));

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    bool inBounds(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    Color at(int x, int y) const;
    void setPixelStrict(int x, int y, const Color& color);

    bool setPixel(int x, int y, const Color& color);
    Color getPixel(int x, int y) const;

    void clear(const Color& color);

    const std::vector<Color>& getPixels() const { return pixels; }
};
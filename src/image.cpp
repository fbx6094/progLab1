#include "image.h"

#include <string>

Image::Image(int w, int h, Color background) {
    if (w <= 0 || h <= 0) {
        throw std::invalid_argument(
            "width and height must be int and >0: ");
    }
    width = w;
    height = h;
    pixels.assign(width * height, background);
}

Color Image::at(int x, int y) const {
    if (!inBounds(x, y)) {
        throw std::out_of_range("coords (" + std::to_string(x) + ", " +
                                std::to_string(y) +
                                ") are out of bounds of the image");
    }
    return pixels[getIndex(x, y)];
}

void Image::setPixelStrict(int x, int y, const Color& color) {
    if (!inBounds(x, y)) {
        throw std::out_of_range("coords (" + std::to_string(x) + ", " +
                                std::to_string(y) +
                                ") are out of bounds of the image");
    }
    pixels[getIndex(x, y)] = color;
}

bool Image::setPixel(int x, int y, const Color& color) {
    if (!inBounds(x, y)) {
        return false;
    }
    pixels[getIndex(x, y)] = color;
    return true;
}

Color Image::getPixel(int x, int y) const {
    if (!inBounds(x, y)) {
        return Color(0, 0, 0);
    }
    return pixels[getIndex(x, y)];
}

void Image::clear(const Color& color) {
    std::fill(pixels.begin(), pixels.end(), color);
}
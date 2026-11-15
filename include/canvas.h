#pragma once
#include "color.h"
#include "image.h"

class Canvas {
   private:
    Image& image;     
    Color penColor;   
    Color fillColor;  

   public:
    explicit Canvas(Image& targetImage)
        : image(targetImage), penColor(255, 255, 255), fillColor(0, 0, 0) {}

    int getWidth() const { return image.getWidth(); }
    int getHeight() const { return image.getHeight(); }
    Image& getImage() { return image; }
    const Image& getImage() const { return image; }

    void setPenColor(const Color& c) { penColor = c; }
    Color getPenColor() const { return penColor; }

    void setFillColor(const Color& c) { fillColor = c; }
    Color getFillColor() const { return fillColor; }

    void clear(const Color& c) { image.clear(c); }

    void drawPixel(int x, int y, const Color& c) {
        if (!image.inBounds(x, y)) {
            return; 
        }
        Color current = image.getPixel(x, y);
        Color blended = current.blend(c);
        image.setPixel(x, y, blended);
    }

    void drawPixel(int x, int y) { drawPixel(x, y, penColor); }
};
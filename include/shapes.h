#pragma once
#include <stdexcept>
#include <vector>
#include <stack>

#include "color.h"
#include "shape.h"

class Rectangle : public IShape {
   private:
    int cx = 0;
    int cy = 0;
    int width = 0;
    int height = 0;
    Color outlineColor;
    Color fillColor;
    int thickness = 1;
    bool isFilled = false;

   public:
    Rectangle(int cx, int cy, int w, int h, Color outline,
              Color fill = Color(0, 0, 0), int thick = 1, bool filled = false);

    void draw(Canvas& canvas) const override;
};

class NestedSquares : public IShape {
   private:
    int cx = 0;
    int cy = 0;
    int startSide = 0;
    int count = 0;
    int step = 0;
    Color startColor;
    Color endColor;
    int thickness = 1;

   public:
    NestedSquares(int cx, int cy, int side, int count, int step, Color startCol,
                  Color endCol, int thick = 1);

    void draw(Canvas& canvas) const override;
};

class LineSegment : public IShape {
   private:
    int x0 = 0;
    int y0 = 0;
    int x1 = 0;
    int y1 = 0;
    Color color;
    int thickness = 1;
    bool useWu = false;

    void drawWu(Canvas& canvas) const;
    void drawBresenham(Canvas& canvas) const;

   public:
    LineSegment(int x0, int y0, int x1, int y1, Color color, int thickness = 1,
                bool useWu = false);

    void draw(Canvas& canvas) const override;
};

struct Point {
    int x = 0;
    int y = 0;
};

class Polygon : public IShape {
   private:
    std::vector<Point> points;
    Color outlineColor;
    int thickness = 1;
    bool useWu = false;

   public:
    Polygon(const std::vector<Point>& pts, Color outline, int thick = 1,
            bool wu = false);

    void draw(Canvas& canvas) const override;
};

class FloodFill : public IShape {
   private:
    int seedX = 0;
    int seedY = 0;
    Color fillColor;
    double tolerance = 0.0;

   public:
    FloodFill(int x, int y, Color fill, double tol = 0.0);

    void draw(Canvas& canvas) const override;
};

class TruchetBoard : public IShape {
   private:
    int cellsX = 8;
    int cellsY = 8;
    Color color0;
    Color color1;
    Color arcColor;
    int arcThickness = 2;

    void drawArcWu(Canvas& canvas, int cx, int cy, int radius, int minX,
                   int maxX, int minY, int maxY) const;

   public:
    TruchetBoard(int nx, int ny, Color c0, Color c1, Color arcCol,
                 int thick = 2);

    void draw(Canvas& canvas) const override;
};

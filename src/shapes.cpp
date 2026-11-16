#include "shapes.h"

Rectangle::Rectangle(int cx, int cy, int w, int h, Color outline, Color fill,
                     int thick, bool filled)
    : cx(cx),
      cy(cy),
      outlineColor(outline),
      fillColor(fill),
      thickness(thick),
      isFilled(filled) {
    if (w <= 0 || h <= 0) {
        throw std::invalid_argument("height and width must be > 0");
    }
    if (thick < 1) {
        throw std::invalid_argument("thick must be >= 1");
    }
    width = w;
    height = h;
}

void Rectangle::draw(Canvas& canvas) const {
    int xMin = cx - width / 2;
    int yMin = cy - height / 2;
    int xMax = xMin + width - 1;
    int yMax = yMin + height - 1;

    if (isFilled) {
        for (int y = yMin; y <= yMax; ++y) {
            for (int x = xMin; x <= xMax; ++x) {
                canvas.drawPixel(x, y, fillColor);
            }
        }
    }

    for (int t = 0; t < thickness; ++t) {
        for (int x = xMin; x <= xMax; ++x) {
            canvas.drawPixel(x, yMin + t, outlineColor);
            canvas.drawPixel(x, yMax - t, outlineColor);
        }

        for (int y = yMin; y <= yMax; ++y) {
            canvas.drawPixel(xMin + t, y, outlineColor);
            canvas.drawPixel(xMax - t, y, outlineColor);
        }
    }
}

NestedSquares::NestedSquares(int cx, int cy, int side, int count, int step,
                             Color startCol, Color endCol, int thick)
    : cx(cx),
      cy(cy),
      startSide(side),
      count(count),
      step(step),
      startColor(startCol),
      endColor(endCol),
      thickness(thick) {
    if (side <= 0 || count <= 0) {
        throw std::invalid_argument("side and count must be > 0");
    }
    if (step < 0) {
        throw std::invalid_argument("step cannot be negative");
    }
    if (thick < 1) {
        throw std::invalid_argument("thickness must be >= 1");
    }
}

void NestedSquares::draw(Canvas& canvas) const {
    for (int i = 0; i < count; ++i) {
        int currentSide = startSide - 2 * i * step;
        if (currentSide <= 0) {
            break;
        }

        double t = 0.0;
        if (count > 1) {
            t = static_cast<double>(i) / static_cast<double>(count - 1);
        }

        Color currentColor = startColor.lerp(endColor, t);

        Rectangle rect(cx, cy, currentSide, currentSide, currentColor,
                       Color(0, 0, 0), thickness, false);
        rect.draw(canvas);
    }
}

LineSegment::LineSegment(int x0, int y0, int x1, int y1, Color color,
                         int thickness, bool useWu)
    : x0(x0),
      y0(y0),
      x1(x1),
      y1(y1),
      color(color),
      thickness(thickness),
      useWu(useWu) {
    if (thickness < 1) {
        throw std::invalid_argument("thickness must be >= 1");
    }
}

void LineSegment::draw(Canvas& canvas) const {
    if (useWu) {
        drawWu(canvas);
    } else {
        drawBresenham(canvas);
    }
}

void LineSegment::drawBresenham(Canvas& canvas) const {
    int startX = x0;
    int startY = y0;
    int endX = x1;
    int endY = y1;

    int dx = std::abs(endX - startX);
    int dy = std::abs(endY - startY);
    int sx = (startX < endX) ? 1 : -1;
    int sy = (startY < endY) ? 1 : -1;
    int err = dx - dy;

    int radius = (thickness - 1) / 2;

    while (true) {
        if (thickness == 1) {
            canvas.drawPixel(startX, startY, color);
        } else {
            for (int oy = -radius; oy <= radius; ++oy) {
                for (int ox = -radius; ox <= radius; ++ox) {
                    if (ox * ox + oy * oy <= radius * radius + radius) {
                        canvas.drawPixel(startX + ox, startY + oy, color);
                    }
                }
            }
        }

        if (startX == endX && startY == endY) {
            break;
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            startX += sx;
        }
        if (e2 < dx) {
            err += dx;
            startY += sy;
        }
    }
}

void LineSegment::drawWu(Canvas& canvas) const {
    double startX = x0;
    double startY = y0;
    double endX = x1;
    double endY = y1;

    bool steep = std::abs(endY - startY) > std::abs(endX - startX);

    if (steep) {
        std::swap(startX, startY);
        std::swap(endX, endY);
    }
    if (startX > endX) {
        std::swap(startX, endX);
        std::swap(startY, endY);
    }

    double dx = endX - startX;
    double dy = endY - startY;
    double gradient = (dx == 0.0) ? 1.0 : dy / dx;

    auto plot = [&](int x, int y, double brightness) {
        brightness = limit(brightness, 0.0, 1.0);
        Color c(color.r, color.g, color.b,
                static_cast<int>(color.a * brightness));
        if (steep) {
            canvas.drawPixel(y, x, c);
        } else {
            canvas.drawPixel(x, y, c);
        }
    };

    double xend = std::round(startX);
    double yend = startY + gradient * (xend - startX);
    double xgap = 1.0 - (startX + 0.5 - std::floor(startX + 0.5));
    int xpxl1 = static_cast<int>(xend);
    int ypxl1 = static_cast<int>(std::floor(yend));

    plot(xpxl1, ypxl1, (1.0 - (yend - std::floor(yend))) * xgap);
    plot(xpxl1, ypxl1 + 1, (yend - std::floor(yend)) * xgap);
    double intery = yend + gradient;

    xend = std::round(endX);
    yend = endY + gradient * (xend - endX);
    xgap = endX + 0.5 - std::floor(endX + 0.5);
    int xpxl2 = static_cast<int>(xend);
    int ypxl2 = static_cast<int>(std::floor(yend));

    plot(xpxl2, ypxl2, (1.0 - (yend - std::floor(yend))) * xgap);
    plot(xpxl2, ypxl2 + 1, (yend - std::floor(yend)) * xgap);

    for (int x = xpxl1 + 1; x < xpxl2; ++x) {
        plot(x, static_cast<int>(std::floor(intery)),
             1.0 - (intery - std::floor(intery)));
        plot(x, static_cast<int>(std::floor(intery)) + 1,
             intery - std::floor(intery));
        intery += gradient;
    }
}

Polygon::Polygon(const std::vector<Point>& pts, Color outline, int thick,
                 bool wu)
    : points(pts), outlineColor(outline), thickness(thick), useWu(wu) {
    if (pts.size() < 3) {
        throw std::invalid_argument("polygon must have at least 3 points");
    }
    if (thick < 1) {
        throw std::invalid_argument("thickness must be >= 1");
    }
}

void Polygon::draw(Canvas& canvas) const {
    size_t n = points.size();
    for (size_t i = 0; i < n; ++i) {
        const Point& p1 = points[i];
        const Point& p2 = points[(i + 1) % n];
        LineSegment line(p1.x, p1.y, p2.x, p2.y, outlineColor, thickness, useWu);
        line.draw(canvas);
    }
}

FloodFill::FloodFill(int x, int y, Color fill, double tol)
    : seedX(x), seedY(y), fillColor(fill), tolerance(tol) {
    if (tol < 0.0) {
        throw std::invalid_argument("tolerance cannot be negative");
    }
}

void FloodFill::draw(Canvas& canvas) const {
    Image& img = canvas.getImage();
    int w = img.getWidth();
    int h = img.getHeight();

    if (!img.inBounds(seedX, seedY)) {
        return;
    }

    Color targetColor = img.getPixel(seedX, seedY);
    if (targetColor == fillColor) {
        return;
    }

    std::vector<bool> visited(static_cast<size_t>(w) * static_cast<size_t>(h),
                              false);
    std::stack<Point> st;

    st.push({seedX, seedY});
    visited[seedY * w + seedX] = true;

    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    while (!st.empty()) {
        Point p = st.top();
        st.pop();

        img.setPixel(p.x, p.y, fillColor);

        for (int i = 0; i < 4; ++i) {
            int nx = p.x + dx[i];
            int ny = p.y + dy[i];

            if (img.inBounds(nx, ny)) {
                size_t idx = static_cast<size_t>(ny) * static_cast<size_t>(w) +
                             static_cast<size_t>(nx);
                if (!visited[idx]) {
                    Color cur = img.getPixel(nx, ny);
                    if (cur.distance(targetColor) <= tolerance) {
                        visited[idx] = true;
                        st.push({nx, ny});
                    }
                }
            }
        }
    }
}

TruchetBoard::TruchetBoard(int nx, int ny, Color c0, Color c1, Color arcCol,
                           int thick)
    : cellsX(nx),
      cellsY(ny),
      color0(c0),
      color1(c1),
      arcColor(arcCol),
      arcThickness(thick) {
    if (nx <= 0 || ny <= 0) {
        throw std::invalid_argument("cell counts must be > 0");
    }
    if (thick < 1) {
        throw std::invalid_argument("thickness must be >= 1");
    }
}

void TruchetBoard::drawArcWu(Canvas& canvas, int cx, int cy, int radius,
                             int minX, int maxX, int minY, int maxY) const {
    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            double dist = std::sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
            double diff = std::abs(dist - radius);
            double halfThick = arcThickness / 2.0;
            if (diff <= (halfThick + 0.5)) {
                double factor = 1.0;
                if (diff > halfThick - 0.5) {
                    factor = 1.0 - (diff - (halfThick - 0.5));
                }
                factor = limit(factor, 0.0, 1.0);
                Color pixelColor(arcColor.r, arcColor.g, arcColor.b,
                                 static_cast<int>(arcColor.a * factor));
                canvas.drawPixel(x, y, pixelColor);
            }
        }
    }
}

void TruchetBoard::draw(Canvas& canvas) const {
    int w = canvas.getWidth();
    int h = canvas.getHeight();
    int cellW = w / cellsX;
    int cellH = h / cellsY;

    for (int j = 0; j < cellsY; ++j) {
        for (int i = 0; i < cellsX; ++i) {
            int x0 = i * cellW;
            int y0 = j * cellH;
            int x1 = (i == cellsX - 1) ? w - 1 : (x0 + cellW - 1);
            int y1 = (j == cellsY - 1) ? h - 1 : (y0 + cellH - 1);

            Color cellBg = ((i + j) % 2 == 0) ? color0 : color1;
            for (int y = y0; y <= y1; ++y) {
                for (int x = x0; x <= x1; ++x) {
                    canvas.drawPixel(x, y, cellBg);
                }
            }

            int radius = std::min(cellW, cellH) / 2;
            int orientation = (i * j) % 2;

            if (orientation == 0) {
                drawArcWu(canvas, x0, y0, radius, x0, x1, y0, y1);
                drawArcWu(canvas, x1, y1, radius, x0, x1, y0, y1);
            } else {
                drawArcWu(canvas, x1, y0, radius, x0, x1, y0, y1);
                drawArcWu(canvas, x0, y1, radius, x0, x1, y0, y1);
            }
        }
    }
}

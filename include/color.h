#pragma once
#include <algorithm>
#include <cmath>
#include <iostream>

template <typename T>
T limit(T value, T min, T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

struct Color {
    int r = 0;
    int g = 0;
    int b = 0;
    int a = 255;

    Color() : r(0), g(0), b(0), a(255) {}
    Color(int r, int g, int b, int a = 255)
        : r(limit(r, 0, 255)),
          g(limit(g, 0, 255)),
          b(limit(b, 0, 255)),
          a(limit(a, 0, 255)) {}

    int togray() const {
        return static_cast<int>(std::round(0.299 * r + 0.587 * g + 0.114 * b));
    }

    Color lerp(const Color& other, double t) const {
        t = limit(t, 0.0, 1.0);
        int nr = static_cast<int>(std::round(r + (other.r - r) * t));
        int ng = static_cast<int>(std::round(g + (other.g - g) * t));
        int nb = static_cast<int>(std::round(b + (other.b - b) * t));
        int na = static_cast<int>(std::round(a + (other.a - a) * t));
        return Color(nr, ng, nb, na);
    }

    Color blend(const Color& src) const {
        double srcalpha = src.a / 255.0;
        double dstalpha = a / 255.0;
        double outalpha = srcalpha + dstalpha * (1 - srcalpha);

        if (outalpha <= 0.0001) {
            return Color(0, 0, 0, 0);
        }
        int nr = static_cast<int>(std::round(
            (src.r * srcalpha + r * dstalpha * (1.0 - srcalpha)) / outalpha));
        int ng = static_cast<int>(std::round(
            (src.g * srcalpha + g * dstalpha * (1.0 - srcalpha)) / outalpha));
        int nb = static_cast<int>(std::round(
            (src.b * srcalpha + b * dstalpha * (1.0 - srcalpha)) / outalpha));
        int na = static_cast<int>(std::round(outalpha * 255.0));
        return Color(nr, ng, nb, na);
    }

    double distance(const Color& other) const{
        double dr = r - other.r;
        double dg = g - other.g;
        double db = b - other.b;

        return std::sqrt(dr*dr + dg*dg + db*db);
    }

    bool operator==(const Color& other) const{
        return r==other.r && g==other.g && b==other.b && a==other.a;
    }

    bool operator!=(const Color& other) const{
        return !(*this == other);
    }
};
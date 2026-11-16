#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "canvas.h"
#include "color.h"
#include "exporter.h"
#include "image.h"
#include "shapes.h"

Color readColor(const std::string& prompt) {
    std::cout << prompt << " (R G B 0..255): ";
    int r = 0, g = 0, b = 0;
    if (!(std::cin >> r >> g >> b)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw std::invalid_argument("invalid color values");
    }
    return Color(r, g, b);
}

void saveImage(const Image& img) {
    std::cout << "Choose file format (1 - PPM P3, 2 - PGM P2): ";
    int fmt = 1;
    if (!(std::cin >> fmt)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw std::invalid_argument("invalid format choice");
    }

    std::cout << "Enter output filename: ";
    std::string filename;
    std::cin >> filename;

    std::unique_ptr<IPNMExporter> exporter;
    if (fmt == 2) {
        exporter = std::make_unique<P2Exporter>();
    } else {
        exporter = std::make_unique<P3Exporter>();
    }

    exporter->exportToFile(img, filename);
    std::cout << "[Success] Image saved to " << filename << " ("
              << img.getWidth() << "x" << img.getHeight() << ")" << std::endl;
}

void handleRectangleAndSquares() {
    int w = 512, h = 512;
    std::cout << "Canvas width and height: ";
    if (!(std::cin >> w >> h)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw std::invalid_argument("invalid dimensions");
    }

    Color bg = readColor("Background color");
    Image img(w, h, bg);
    Canvas canvas(img);

    std::cout << "1 - Single Rectangle, 2 - Nested Squares: ";
    int sub = 1;
    std::cin >> sub;

    if (sub == 1) {
        int cx = w / 2, cy = h / 2, rw = 200, rh = 150, thick = 2, filled = 0;
        std::cout << "Enter center cx cy: ";
        std::cin >> cx >> cy;
        std::cout << "Enter width and height: ";
        std::cin >> rw >> rh;
        std::cout << "Border thickness: ";
        std::cin >> thick;
        std::cout << "Filled (1 - yes, 0 - no): ";
        std::cin >> filled;

        Color outline = readColor("Outline color");
        Color fill = Color(0, 0, 0);
        if (filled) {
            fill = readColor("Fill color");
        }

        Rectangle rect(cx, cy, rw, rh, outline, fill, thick, filled != 0);
        rect.draw(canvas);
    } else {
        int cx = w / 2, cy = h / 2, side = 300, count = 10, step = 15, thick = 2;
        std::cout << "Enter center cx cy: ";
        std::cin >> cx >> cy;
        std::cout << "Start side, count, step: ";
        std::cin >> side >> count >> step;
        std::cout << "Line thickness: ";
        std::cin >> thick;

        Color startCol = readColor("Start color (outer)");
        Color endCol = readColor("End color (inner)");

        NestedSquares ns(cx, cy, side, count, step, startCol, endCol, thick);
        ns.draw(canvas);
    }

    saveImage(img);
}

void handleLineSegment() {
    int w = 512, h = 512;
    std::cout << "Canvas width and height: ";
    if (!(std::cin >> w >> h)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw std::invalid_argument("invalid dimensions");
    }

    Color bg = readColor("Background color");
    Image img(w, h, bg);
    Canvas canvas(img);

    int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
    std::cout << "Point A (x0 y0): ";
    std::cin >> x0 >> y0;
    std::cout << "Point B (x1 y1): ";
    std::cin >> x1 >> y1;

    std::cout << "Algorithm (1 - Wu Anti-Aliased, 2 - Bresenham Thick): ";
    int algo = 1;
    std::cin >> algo;

    int thick = 1;
    if (algo == 2) {
        std::cout << "Line thickness: ";
        std::cin >> thick;
    }

    Color lineCol = readColor("Line color");

    LineSegment line(x0, y0, x1, y1, lineCol, thick, algo == 1);
    line.draw(canvas);

    saveImage(img);
}

void handleFloodFill() {
    int w = 512, h = 512;
    std::cout << "Canvas width and height: ";
    if (!(std::cin >> w >> h)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw std::invalid_argument("invalid dimensions");
    }

    Color bg = readColor("Background color");
    Image img(w, h, bg);
    Canvas canvas(img);

    std::cout << "Draw polygon boundary first? (1 - yes, 0 - no): ";
    int drawPoly = 1;
    std::cin >> drawPoly;

    if (drawPoly) {
        std::vector<Point> pts = {{w / 4, h / 4},
                                  {3 * w / 4, h / 4},
                                  {5 * w / 6, 3 * h / 4},
                                  {w / 6, 3 * h / 4}};
        Color polyCol = readColor("Polygon outline color");
        Polygon poly(pts, polyCol, 2, false);
        poly.draw(canvas);
        std::cout << "[Info] Polygon drawn with seed inside at (" << w / 2 << ", "
                  << h / 2 << ")" << std::endl;
    }

    int sx = w / 2, sy = h / 2;
    std::cout << "Seed point (x y): ";
    std::cin >> sx >> sy;

    double tol = 0.0;
    std::cout << "Color tolerance (epsilon, e.g. 0.0 or 25.0): ";
    std::cin >> tol;

    Color fillCol = readColor("Fill color");

    FloodFill ff(sx, sy, fillCol, tol);
    ff.draw(canvas);

    saveImage(img);
}

void handleTruchetBoard() {
    int w = 600, h = 600;
    std::cout << "Canvas width and height: ";
    if (!(std::cin >> w >> h)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw std::invalid_argument("invalid dimensions");
    }

    int nx = 8, ny = 8;
    std::cout << "Cells count along X and Y (e.g. 8 8): ";
    std::cin >> nx >> ny;

    Color c0 = readColor("Chess cell color 0");
    Color c1 = readColor("Chess cell color 1");
    Color arcCol = readColor("Truchet arc color");

    int thick = 3;
    std::cout << "Arc thickness (e.g. 3): ";
    std::cin >> thick;

    Image img(w, h, Color(0, 0, 0));
    Canvas canvas(img);

    TruchetBoard board(nx, ny, c0, c1, arcCol, thick);
    board.draw(canvas);

    saveImage(img);
}

int main() {
    while (true) {
        std::cout << "\n=== Netpbm Graphics Engine (C++17) ===" << std::endl;
        std::cout << "1. Rectangle and Nested Squares" << std::endl;
        std::cout << "2. Line Segment (Wu Anti-Aliased / Thick Bresenham)" << std::endl;
        std::cout << "3. Flood Fill and Polygon" << std::endl;
        std::cout << "4. Variant 2: Truchet Tiling Chessboard" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Select menu item: ";

        int choice = -1;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        if (choice == 0) {
            std::cout << "Exiting application." << std::endl;
            break;
        }

        try {
            switch (choice) {
                case 1:
                    handleRectangleAndSquares();
                    break;
                case 2:
                    handleLineSegment();
                    break;
                case 3:
                    handleFloodFill();
                    break;
                case 4:
                    handleTruchetBoard();
                    break;
                default:
                    std::cout << "Unknown option. Try again." << std::endl;
                    break;
            }
        } catch (const std::exception& e) {
            std::cerr << "[Error]: " << e.what() << std::endl;
        }
    }

    return 0;
}

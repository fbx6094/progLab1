#include <iostream>
    #include "image.h"
    #include "canvas.h"
    #include "exporter.h"

    int main() {
        std::cout << "=== Test ===" << std::endl;

        try {
            Image img(200, 200, Color(20, 20, 40));
            Canvas canvas(img);

            for (int i = 0; i < 150; ++i) {
                canvas.drawPixel(20 + i, 20 + i, Color(255, 50, 50));
            }

            P3Exporter exporter;
            exporter.exportToFile(img, "test.ppm");

            std::cout << "file test.ppm created successfully!" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "error: " << e.what() << std::endl;
            return 1;
        }

        return 0;
    }
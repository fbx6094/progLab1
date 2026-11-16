#include "exporter.h"

#include <fstream>
#include <stdexcept>

void P2Exporter::exportToFile(const Image& image,
                              const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) {
        throw std::runtime_error("couldnt open file to write: " +
                                 filename);
    }

    int width = image.getWidth();
    int height = image.getHeight();


    out << "P2\n";
    out << width << " " << height << "\n";
    out << "255\n";

    int currentLineLength = 0;
    const auto& pixels = image.getPixels();

    for (const auto& pixel : pixels) {
        std::string valStr = std::to_string(pixel.toGrayscale());

        if (currentLineLength + static_cast<int>(valStr.length()) + 1 > 70) {
            out << "\n";
            currentLineLength = 0;
        } else if (currentLineLength > 0) {
            out << " ";
            currentLineLength += 1;
        }

        out << valStr;
        currentLineLength += static_cast<int>(valStr.length());
    }
    out << "\n";
}


void P3Exporter::exportToFile(const Image& image,
                              const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) {
        throw std::runtime_error("couldnt open file to write: " +
                                 filename);
    }

    int width = image.getWidth();
    int height = image.getHeight();

    out << "P3\n";
    out << width << " " << height << "\n";
    out << "255\n";

    int currentLineLength = 0;
    const auto& pixels = image.getPixels();

    for (const auto& pixel : pixels) {
        // Тройка R G B
        std::string triplet = std::to_string(pixel.r) + " " +
                              std::to_string(pixel.g) + " " +
                              std::to_string(pixel.b);
        if (currentLineLength + static_cast<int>(triplet.length()) + 1 > 70) {
            out << "\n";
            currentLineLength = 0;
        } else if (currentLineLength > 0) {
            out << " ";
            currentLineLength += 1;
        }

        out << triplet;
        currentLineLength += static_cast<int>(triplet.length());
    }
    out << "\n";
}
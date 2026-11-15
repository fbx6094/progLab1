#pragma once
#include <string>

#include "image.h"

class IPNMExporter {
   public:
    virtual ~IPNMExporter() = default;

    virtual void exportToFile(const Image& image,
                              const std::string& filename) const = 0;
};

class P2Exporter : public IPNMExporter {
   public:
    void exportToFile(const Image& image,
                      const std::string& filename) const override;
};

class P3Exporter : public IPNMExporter {
   public:
    void exportToFile(const Image& image,
                      const std::string& filename) const override;
};
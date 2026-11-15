#pragma once
#include "canvas.h"
class IShape {
   public:
    virtual ~IShape() = default;
    virtual void draw(Canvas& canvas) const = 0;
};
#pragma once

#include "Linalg.h"

#include <vector>

namespace renderer {
struct Triangle {
    Matrix3d points;
};

struct Object {
    std::vector<Triangle> trisngles;
};
} // namespace renderer
